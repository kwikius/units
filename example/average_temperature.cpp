
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>

#include <units/dimensions/temperature.h>

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

using namespace units;

namespace {

    typedef units::quantity<units::kelvin> K;
    double filter_k = 0.8;

    K current_temperature{270};
}

void update_temperature(std::vector<units::quantity<units::kelvin> > const & v)
{
   current_temperature = std::accumulate(v.begin(),v.end(),current_temperature,filter{filter_k});
}

int main()
{
  
  std::cout << current_temperature <<'\n';

  std::vector<K> new_values = {K{270},K{271},K{270},K{271},K{270},K{270}};

  update_temperature(new_values);

  std::cout << current_temperature <<'\n';

}