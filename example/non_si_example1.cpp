
#include <units/dimensions/mass.h>
#include <units/dimensions/length.h>
#include <units/dimensions/time.h>

/*
 include  forces output of force unit, energy unit
*/
//#include <units/dimensions/energy.h>

#include <iostream>

int main()
{
  using namespace units;

  constexpr quantity<foot> distance(220);
  constexpr quantity<hour> time(2);
  constexpr quantity<minute> time1(4);
  constexpr quantity<kilogram> mass{20};

  constexpr quantity<yard> distance2(10);

  constexpr auto speed = distance/ time;

  constexpr auto acc = speed / time1;
 
  constexpr auto force = mass * acc;

  constexpr auto q = force * distance2;

  std::cout << speed << '\n';
  std::cout << acc << '\n';
  std::cout << force << '\n';

  // fails in output atm
 // std::cout << q <<'\n';

}