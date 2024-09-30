#include "Delaunay.h"

namespace gam
{
  std::pair<Point, ScalarType> circumcircle(const Point& a, const Point& b, const Point& c)
  {
    Point center; 

    Vector ab(a, b);
    Vector ac(a, c);
    Vector bc(b, c);

    ab.normalize();
    ac.normalize();
    bc.normalize();

    ScalarType sin_a= (ab.cross(ac)).norm();
    ScalarType cos_a= ab.dot(ac);
    ScalarType sin_2a= 2 * sin_a * cos_a;

    ScalarType sin_b= (bc.cross(-ab)).norm();
    ScalarType cos_b= bc.dot(-ab);
    ScalarType sin_2b= 2 * sin_b * cos_b;

    ScalarType sin_c= (-ac.cross(-bc)).norm();
    ScalarType cos_c= -ac.dot(-bc);
    ScalarType sin_2c= 2 * sin_c * cos_c;

    center.X= (a.X * sin_2a + b.X * sin_2b + c.X * sin_2c) / (sin_2a + sin_2b + sin_2c);
    center.Y= (a.Y * sin_2a + b.Y * sin_2b + c.Y * sin_2c) / (sin_2a + sin_2b + sin_2c);
    center.Z= (a.Z * sin_2a + b.Z * sin_2b + c.Z * sin_2c) / (sin_2a + sin_2b + sin_2c);

    ScalarType radius= (Vector(center, a)).norm();

    return {center, radius};
  }

} // namespace gam