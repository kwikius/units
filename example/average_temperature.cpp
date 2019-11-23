
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

int main()
{
  
  using namespace units;

  typedef quantity<units::kelvin> K;

  std::vector<K> ts = {K{270},K{271},K{270},K{271},K{270},K{270}};

  double filter_k = 0.8;

  K average = std::accumulate(ts.begin(),ts.end(),ts.front(),filter{filter_k});

  std::cout << average << '\n';

}