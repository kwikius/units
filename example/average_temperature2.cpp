
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
//https://en.wikipedia.org/wiki/Affine_combination

//https://en.wikipedia.org/wiki/Affine_combination

template <typename T>
T average(std::vector<T> const & v )
{
   assert(v.size() > 0);
   return std::accumulate(v.begin(),v.end(),T{})/ v.size();
}

using namespace units;

namespace {

   typedef quantity<kelvin> K;

}

int main()
{
   std::vector<K> values = {K{270},K{271},K{270},K{271},K{270},K{270}};

   std::cout << average(values) << '\n';
}
