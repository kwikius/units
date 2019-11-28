
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>

#include <units/dimensions/temperature.h>
#include <units/dimensions/time.h>


namespace units {
    struct fahrenheit : named_scaled_derived_unit<fahrenheit, temperature, "F", ratio<5, 9>> {};
    namespace literals {
    constexpr auto operator""_F(unsigned long long l) { return quantity<fahrenheit, std::int64_t>(l); }
    constexpr auto operator""_F(long double l) { return quantity<fahrenheit, long double>(l); }   // TODO: conflicts with gcc GNU extension

    } // namespace literals
} // namespace units



using namespace units;

namespace detail {

template <typename Unit>
struct scale_reference_point {};

template <typename Quantity>
static constexpr quantity<kelvin> scale_reference_point_v = scale_reference_point<typename Quantity::unit>::value;

template <>
struct scale_reference_point<fahrenheit> {
    static constexpr quantity<kelvin> value = 255.372_K;
};

template <>
struct scale_reference_point<kelvin> {
    static constexpr quantity<kelvin> value = 0._K;
};

} // namespace detail

template <typename T>
struct is_absolute : std::false_type {};

template <typename T>
static constexpr bool is_absolute_v = is_absolute<T>::value;

template <typename Unit>
struct absolute {
    public:
        using offset_type = Unit;

        explicit constexpr absolute(offset_type const &v) : offset(v) {}

        template <typename V>
        constexpr std::enable_if_t<!is_absolute_v<V>,absolute> operator+(V const &delta) const {
            return absolute(offset + delta);
        }
        template <typename V>
        constexpr std::enable_if_t<!is_absolute_v<V>,absolute> operator-(V const &delta) const {
            return absolute(offset - delta);
        }
        template <typename V>
        constexpr std::enable_if_t<is_absolute_v<V>,offset_type> operator-(V const &other) const {
            return offset - other.template to_scale<offset_type>().offset_to_reference();
        }

        template <typename T>
        constexpr absolute<T> to_scale() const {
            return absolute<T>(
                offset + (
                    ::detail::scale_reference_point_v<offset_type>
                    - ::detail::scale_reference_point_v<T>
                )
            );
        } 

        constexpr offset_type offset_to_reference() const {
            return offset;
        }

    private:
        offset_type offset;  // offset from reference value
};

template <typename T>
struct is_absolute<absolute<T>> : std::true_type {};

/*
   get average temperature
   ( argument for explicit affine quantities)
*/

// https://en.wikipedia.org/wiki/Exponential_smoothing

struct filter{
   filter(double k): m_k{k}{ assert( (0 <= k) && (k <= 1));}
   template <typename T, typename R>
   T operator()( T const & lhs, R const & rhs)const 
   {
    #if 1
      // This will fail at compile time
      // return lhs * m_k + rhs * (1.0-m_k);
      // absolute quantities cant be added directly so
      // need a bit of persuasion
      // subtracting 2 absolute quantities returns a quantity_difference 
      auto zero = T{lhs - lhs};
      // quantity_differences can multiplied and be added
      auto v = (lhs - zero) * m_k + (rhs-zero) * (1.0-m_k);
      // quantity_difference can also be added to absolute quantity 
      return zero + v;
    #else
      // If you see the rearrangement of the equation, this is the right way to do it
      return lhs + (rhs-lhs) * (1.0-m_k);
    #endif
   }
   private:
      double const m_k;
 };

template <typename T, typename T1>
void update(T1 & out, std::vector<T> const & v , double const & filter_k)
{
   out = std::accumulate(v.begin(),v.end(),out,filter{filter_k});
}


namespace {

   typedef absolute<quantity<kelvin>> K;
   typedef absolute<quantity<fahrenheit>> F;
   typedef quantity<second> s;

   double filter_k = 0.9;

   K current_temperature(270.0_K);
   s current_time = 1000.0s;

}

int main()
{

   std::vector<F> new_values_F = {F(32._F), F(33._F), F(33._F), F(32._F)};

   auto const old_temperature = current_temperature;
   auto const old_time = current_time;

   for (; current_time < 1005s;  ++current_time){
      update(current_temperature,new_values_F, filter_k);
      std::cout << "temperature at " << current_time << " = " << current_temperature.offset_to_reference() <<'\n';      
   }
   std::vector<K> new_values = {K(270._K),K(271._K),K(270._K),K(271._K),K(270._K),K(270._K)};

   for (; current_time < 1010s;  ++current_time){
      update(current_temperature,new_values, filter_k);
      std::cout << "temperature at " << current_time << " = " << current_temperature.offset_to_reference() <<'\n';      
   }

   std::cout << "rate of temperature change / time = " << (current_temperature - old_temperature) / (current_time - old_time) << '\n';

}