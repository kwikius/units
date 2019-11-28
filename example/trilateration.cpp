
/*
  Trilateration example from Wikipedia https://en.wikipedia.org/wiki/Trilateration
  Tested out with https://github.com/mpusz/units

  also see the Openscad example which really helps to visualise the problem in 3D
  https://github.com/kwikius/Trilateration/blob/master/trilateration.scad

  https://stackoverflow.com/questions/9747227/2d-trilateration
  https://stackoverflow.com/questions/23400351/localizing-a-point-using-distances-to-three-other-points-in-3-d/23401529

  requires my quan library ( headers only required)
  https://github.com/kwikius/quan-trunk
*/

#include <iostream>

#include <units/dimensions/length.h>
#include <units/math.h>

#include <quan/two_d/out/vect.hpp>
#include <quan/three_d/out/vect.hpp>
#include <quan/meta/binary_op.hpp>
#include <quan/meta/is_scalar.hpp>


namespace quan{ namespace meta{ namespace impl{

   /*
      quan has a different view on what a scalar is to mpusz::units.
      No matter, though confusing, they can work together
   */
   template <units::Quantity Q>
   struct is_scalar_impl<Q> : std::true_type{};

   // quan (has been around since 2003) uses specialisations of binary_op_impl
   // for result type deduction on operations. Back then there was no such thing as decltype and auto etc.
   // Though you can use decltype for deduction, there may be some types
   // that dont have any runtime operations so this type may still be useful
   // though maybe one could define the runtime operators on the meta types anyway
   // anyway here is the basic integration to get units::Quantity working with quan::two_d::vector

   template <units::Quantity Lhs, units::Quantity Rhs>
   struct binary_op_impl<Lhs, quan::meta::plus, Rhs>{
      typedef decltype(Lhs{} + Rhs{}) type;
   };

   template <units::Quantity Lhs, units::Quantity Rhs>
   struct binary_op_impl<Lhs, quan::meta::minus, Rhs>{
      typedef decltype(Lhs{} - Rhs{}) type;
   };

   template <units::Quantity Lhs, units::Quantity Rhs>
   struct binary_op_impl<Lhs, quan::meta::times, Rhs>{
      typedef decltype(Lhs{} * Rhs{}) type;
   };

   template <units::Quantity Lhs, units::Scalar Rhs>
   struct binary_op_impl<Lhs, quan::meta::times, Rhs>{
      typedef decltype(Lhs{} * Rhs{}) type;
   };

   template <units::Scalar Lhs, units::Quantity Rhs>
   struct binary_op_impl<Lhs, quan::meta::times, Rhs>{
      typedef decltype(Lhs{} * Rhs{}) type;
   };

   template <units::Quantity Lhs, units::Quantity Rhs>
   struct binary_op_impl<Lhs, quan::meta::divides, Rhs>{
      typedef decltype(Lhs{} / Rhs{}) type;
   };

   template <units::Quantity Lhs, units::Scalar Rhs>
   struct binary_op_impl<Lhs, quan::meta::divides, Rhs>{
      typedef decltype(Lhs{} / Rhs{}) type;
   };

   template <units::Scalar Lhs, units::Quantity Rhs>
   struct binary_op_impl<Lhs, quan::meta::divides, Rhs>{
      typedef decltype(Lhs{} / Rhs{}) type;
   };

}}}

using namespace units;
namespace {

  //N.B at some point use affine quantity type but for now this has to do.
  typedef quan::three_d::vect<quantity<kilometre> > point;

  typedef quan::three_d::vect<quantity<kilometre> > point3D;

  struct sphere{
     point centre;
     quantity<kilometre> radius;
  };

   std::ostream & operator<< ( std::ostream & out, sphere const & c)
   {
      return out << "sphere(centre = " << c.centre << ", radius = " << c.radius << ")";
   }
}

int main()
{
   // TODO matric pre-calc so that spheres can be arbitraily positioned in 3d

   sphere A{{0.0km,0.0km,0.0km},8.0km};
   //sphere A must be centred at origin
   assert( (A.centre == point{0.0km, 0.0km,0.0km}));
   std::cout << "A = " << A <<'\n';

   sphere B{{10.0km,0.0km,0.0km},5.0km};
   // sphere B must be centred on x axis
   assert( (B.centre.y == 0.0km) && (B.centre.z == 0.0km));
   std::cout << "B = " << B <<'\n';

   sphere C{{7.0km,7.0km,0.0km},7.0km};
   // sphere C centre must be on xy plane
   assert(C.centre.z == 0.0km);
   std::cout << "C = " << C <<'\n';

   auto const ex = unit_vector(B.centre-A.centre);

   auto const i = dot_product(ex,(C.centre- A.centre));
   
   auto const ey = unit_vector(C.centre - A.centre - i * ex) ;

   auto const d = magnitude(B.centre - A.centre);

   auto const j = dot_product(ey,C.centre - A.centre);

   auto const x = (pow<2>(A.radius) - pow<2>(B.radius) + pow<2>(d)) / ( 2 * d);

   if ( ((d - A.radius) >= B.radius ) || (B.radius >= (d + A.radius))){
      std::cout << "y : no solution\n";
      return 0;
   }

   auto const y =  (
         ( pow<2>(A.radius) - pow<2>(C.radius) + pow<2>(i) + pow<2>(j))
               / ( 2 * j) 
                  ) - ( i / j) * x;

   auto const z_2 = pow<2>(A.radius) - pow<2>(x) - pow<2>(y);
   if ( z_2 >= pow<2>(0km)){
      auto const z = sqrt(z_2);

      point3D intersection_point {x,y,z};

      std::cout << "intersection point = " << intersection_point << '\n';
     
   }else{
      std::cout << "z : no solution\n";
      return 0;
   }
   return 0;
}
