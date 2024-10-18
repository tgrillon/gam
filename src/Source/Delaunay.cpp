#include "Delaunay.h"

namespace gam
{
  // std::pair<Point, ScalarType> circumcircle(const Point& a, const Point& b, const Point& c)
  // {
  //   Point center; 

  //   Vector ab(a, b);
  //   Vector ac(a, c);
  //   Vector bc(b, c);

  //   ab.normalize();
  //   ac.normalize();
  //   bc.normalize();

  //   ScalarType sin_a= (ab.cross(ac)).norm();
  //   ScalarType cos_a= ab.dot(ac);
  //   ScalarType sin_2a= 2 * sin_a * cos_a;

  //   ScalarType sin_b= (bc.cross(-ab)).norm();
  //   ScalarType cos_b= bc.dot(-ab);
  //   ScalarType sin_2b= 2 * sin_b * cos_b;

  //   ScalarType sin_c= (-ac.cross(-bc)).norm();
  //   ScalarType cos_c= -ac.dot(-bc);
  //   ScalarType sin_2c= 2 * sin_c * cos_c;

  //   center.x= (a.x * sin_2a + b.x * sin_2b + c.x * sin_2c) / (sin_2a + sin_2b + sin_2c);
  //   center.y= (a.y * sin_2a + b.y * sin_2b + c.y * sin_2c) / (sin_2a + sin_2b + sin_2c);
  //   center.z= (a.z * sin_2a + b.z * sin_2b + c.z * sin_2c) / (sin_2a + sin_2b + sin_2c);

  //   ScalarType radius= (Vector(center, a)).norm();

  //   return {center, radius};
  // }

} // namespace gam