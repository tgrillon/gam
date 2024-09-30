#include "geometry.h"

namespace GAM
{
Vector operator*(ScalarType s, const Vector &V)
{
  return Vector(s*V.X, s*V.Y, s*V.Z);
}

Vector operator*(const Vector &V, ScalarType s)
{
  return s * V;
}

Vector operator/(const Vector &V, ScalarType d)
{
  assert(d > 0. || d<0.);
  ScalarType id= 1. / d;
  return id * V;
}

Vector operator/(const Vector &U, const Vector &V)
{
  return Vector(U.X/V.X, U.Y/V.Y, U.Z/V.Z);
}

Vector operator+(const Vector &U, const Vector &V)
{
  return Vector(U.X+V.X, U.Y+V.Y, U.Z+V.Z);
}

Vector operator+=(const Vector &U, const Vector &V)
{
  return U + V;
}

Vector operator-(const Vector &U, const Vector &V)
{
  return Vector(U.X-V.X, U.Y-V.Y, U.Z-V.Z);
}

Vector operator-=(const Vector &U, const Vector &V)
{
  return U - V;
}

std::ostream &operator<<(std::ostream &out, const Vector &V)
{
  out << "Vector : [" << V.X << ", " << V.Y << ", " << V.Z << "]";
  return out;
}

std::ostream &operator<<(std::ostream &out, const Vertex &V)
{
  out << "Vertex : [" << V.X << ", " << V.Y << ", " << V.Z << "]";
  return out;
}

Vector operator/=(const Vector &U, const Vector &V)
{
  assert((V.X > 0. || V.X < 0.) &&
          (V.Y > 0. || V.Y < 0.) && 
          (V.Z > 0. || V.Z < 0.));

  return U / V;
}

Vector operator-(const Vector &U)
{
  Vector ret= U;
  ret.X= -ret.X;
  ret.Y= -ret.Y;
  ret.Z= -ret.Z;
  return ret;
}

Vector operator/=(const Vector &V, ScalarType d)
{
  assert(d > 0 || d < 0);

  return V / d;
}

Point::Point() : X(0.), Y(0.), Z(0.) {}

Point::Point(ScalarType x, ScalarType y, ScalarType z) : X(x), Y(y), Z(z) {}

Point Point::pmin(const Point &a, const Point &b)
{
  return Point(std::min(a.X, b.X), std::min(a.Y, b.Y), std::min(a.Z, b.Z));
}

Point Point::pmax(const Point &a, const Point &b)
{
  return Point(std::max(a.X, b.X), std::max(a.Y, b.Y), std::max(a.Z, b.Z));
}

Point operator+(const Point &a, const Point &b)
{
  return Point(a.X+b.X, a.Y+b.Y, a.Z+b.Z);
}

Point operator+=(const Point &a, const Point &b)
{
  return a + b;
}

Point operator-(const Point &a)
{
  return Point(-a.X, -a.Y, -a.Z);
}

Point operator-(const Point &a, const Point &b)
{
  return Point(a.X-b.X, a.Y-b.Y, a.Z-b.Z);
}

Point operator-=(const Point &a, const Point &b)
{
  return a - b;
}

Point operator*(ScalarType s, const Point &b)
{
  return Point(s*b.X, s*b.Y, s*b.Z);
}

Point operator*(const Point &b, ScalarType s)
{
  return s * b;
}

Point operator*(const Point &a, const Point &b)
{
  return Point(a.X*b.X, a.Y*b.Y, a.Z*b.Z);
}

Point operator*=(const Point &a, const Point &b)
{
  return a * b;
}

Point operator/(const Point &a, ScalarType s)
{
  assert(s > 0. || s < 0.);

  return Point(a.X/s, a.Y/s, a.Z/s);
}

Point operator/=(const Point &a, ScalarType s)
{
  return a / s;
}

Point operator/(const Point &a, const Point &b)
{
  assert((b.X > 0. || a.X < 0.) &&
         (b.Y > 0. || a.Y < 0.) &&
         (b.Z > 0. || a.Z < 0.));

  return Point(a.X/b.X, a.Y/b.Y, a.Z/b.Z);
}

Point operator/=(const Point &a, const Point &b)
{
  return a / b;
}

} // namespace GAM