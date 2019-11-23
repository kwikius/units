
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

  constexpr quantity<metre> distance(220);
  constexpr quantity<second> time(2);
  constexpr quantity<second> time1(4);
  constexpr quantity<kilogram> mass{20};

  constexpr quantity<metre> distance2(10);
//  constexpr Velocity speed = avg_speed(distance, time);
  constexpr auto speed = distance/ time;

  constexpr auto acc = speed / time1;
 
  constexpr auto force = mass * acc;

  constexpr auto torque = force * distance2;

  std::cout << speed << '\n';
  std::cout << acc << '\n';
  std::cout << force << '\n';
  std::cout << torque <<'\n';

}