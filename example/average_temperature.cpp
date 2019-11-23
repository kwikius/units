
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>

#include <units/dimensions/temperature.h>
#include <units/dimensions/time.h>

/*
   get average temperature
   ( argument against getting too rigid about affine quantities)
*/

// https://en.wikipedia.org/wiki/Exponential_smoothing

struct filter{
   filter(double k): m_k{k}{ assert( (0 <= k) && (k <= 1));}
   template <typename T>
   T operator()( T const & lhs, T const & rhs)const 
   {
      return lhs * m_k + rhs * (1.0-m_k);
   }
   private:
      double const m_k;
 };

template <typename T, typename T1>
void update(T1 & out, std::vector<T> const & v , double const & filter_k)
{
   out = std::accumulate(v.begin(),v.end(),out,filter{filter_k});
}

using namespace units;

namespace {

   typedef quantity<kelvin> K;
   typedef quantity<second> s;

   double filter_k = 0.9;

   K current_temperature= 270.0_K;
   s current_time = 1000.0s;

}

int main()
{

   std::vector<K> new_values = {K{270},K{271},K{270},K{271},K{270},K{270}};

   for (; current_time < 1005s;  ++current_time){
      std::cout << "temperature at " << current_time << " = " << current_temperature <<'\n';
      update(current_temperature,new_values, filter_k);
   }

}