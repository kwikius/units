#include <iostream>
#include <units/dimensions/temperature.h>
#include <units/dimensions/time.h>


using namespace units;

int main()
{
   auto dK_dt = 1.0_K / 1s;

   auto initial_temperature = 273.15_K;

   auto t = 10s;

   auto predicted_temperature = initial_temperature + dK_dt * t;

   std::cout << predicted_temperature << '\n';
}