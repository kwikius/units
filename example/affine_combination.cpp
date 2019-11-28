
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>

#include <units/dimensions/temperature.h>

template <typename QSeq, typename KSeq>
decltype(typename QSeq::value_type{} * typename KSeq::value_type{})
affine_combine(QSeq const & values, KSeq const & weights )
{
   assert ( std::abs(std::accumulate(weights.begin(),weights.end(),0.0) - 1.0) < typename KSeq::value_type{1.e-6}); 
   return std::inner_product(values.begin(),values.end(),weights.begin(),typename QSeq::value_type{});
}

template <typename QSeq>
typename QSeq::value_type
affine_combine(QSeq const & values)
{
   assert(values.size() > 0);
   return std::accumulate(values.begin(),values.end(),typename QSeq::value_type{})/ values.size();
}

using namespace units;

namespace {

   typedef quantity<kelvin> K;

}

int main()
{
   std::vector<K> values = {K{270},K{271},K{270},K{271},K{270},K{270}};
   std::vector<double> weights = {0.1,0.2,0.1,0.2,0.25,0.15};

   std::cout << "affine_combine with equal weights: " << affine_combine(values) <<'\n';
   std::cout << "affine_combine with individual weights: " << affine_combine(values,weights) <<'\n';
}