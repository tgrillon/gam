#pragma once

#include "utils.h"

namespace GAM
{
struct Point
{
  ScalarType X, Y, Z; 

  Point();
  Point(ScalarType x, ScalarType y, ScalarType z);

  /// @brief Get the point with the mininmum value between a and b for each x, y and z component.
  /// @return the point with the minimum value between a and b for each component. 
  static Point pmin(const Point& a, const Point& b); 

  /// @brief Get the point with the maximum value between a and b for each x, y and z component.
  /// @return the point with the maximum value between a and b for each component. 
  static Point pmax(const Point& a, const Point& b); 

  friend Point operator+(const Point& a, const Point& b);
  friend Point operator+=(const Point& a, const Point& b);
  friend Point operator-(const Point& a);
  friend Point operator-(const Point& a, const Point& b);
  friend Point operator-=(const Point& a, const Point& b);
  friend Point operator*(ScalarType s, const Point& b);
  friend Point operator*(const Point& b, ScalarType s);
  friend Point operator*(const Point& a, const Point& b);
  friend Point operator*=(const Point& a, const Point& b);
  friend Point operator/(const Point& a, ScalarType s);
  friend Point operator/=(const Point& a, ScalarType s);
  friend Point operator/(const Point& a, const Point& b);
  friend Point operator/=(const Point& a, const Point& b);
};

struct Vertex 
{
  ScalarType X, Y, Z; 
  int FaceIndex { -1 }; 

  inline static Point as_point(const Vertex& V) { return Point(V.X, V.Y, V.Z); }

  friend std::ostream& operator<<(std::ostream& out, const Vertex& V);
};

struct Face
{
  int Vertices[3]  { -1, -1, -1 };
  int Neighbors[3] { -1, -1, -1 };
};

struct Vector 
{
  inline static Vector unit_x() { return {1., 0., 0.}; }
  inline static Vector unit_y() { return {0., 1., 0.}; }
  inline static Vector unit_z() { return {0., 0., 1.}; }
  inline static Vector null()  { return {0., 0., 0.}; }

  Vector() : X(0), Y(0), Z(0) {}
  Vector(ScalarType x, ScalarType y, ScalarType z) : X(x), Y(y), Z(z) {}

  Vector(const Vertex& p, const Vertex& q)
  {	
    X= q.X-p.X;
    Y= q.Y-p.Y;
    Z= q.Z-p.Z;
  }

  Vector(const Point& p, const Point& q)
  {	
    X= q.X-p.X;
    Y= q.Y-p.Y;
    Z= q.Z-p.Z;
  }

  /// @brief Calculate the dot product between `this` and V.
  /// @param V a vector.
  /// @return the value of the dot product.  
  inline ScalarType dot(const Vector& V) const { return X*V.X+Y*V.Y+Z*V.Z; }

  /// @brief Calculate the cross product between `this` and V.
  /// @param V a vector.
  /// @return the orthogonal vector to `this` and V.
  inline Vector cross(const Vector& V) const { return Vector(Y*V.Z-Z*V.Y, Z*V.X-X*V.Z, X*V.Y-Y*V.X); }

  /// @brief Calculate the magnitude²/norm² of `this`.
  /// @return the norm² of `this`.
  inline ScalarType norm2() const { return X*X+Y*Y+Z*Z; };

  /// @brief Calculate the magnitude/norm of `this`.
  /// @return the norm of `this`.
  inline ScalarType norm() const { return std::sqrt(norm2()); }

  /// @brief Normalize `this`.
  inline void normalize() { (*this)= this->normalized(); }

  /// @brief Return a normalize vector.
  /// @return the normalized vector of `this`. 
  inline Vector normalized() const { return (*this)/this->norm(); }

  /// @brief Calculate the cotangent between `this` and V.
  /// @param V a vector.
  /// @return the value of cot(theta) with theta the angle between `this` and V.
  inline ScalarType cotan(const Vector& V) const { return this->dot(V)/(this->cross(V)).norm(); }

  friend Vector operator*(ScalarType s, const Vector& V);
  friend Vector operator*(const Vector& V, ScalarType s);
  friend Vector operator/(const Vector& V, ScalarType d);
  friend Vector operator/=(const Vector& V, ScalarType d);
  friend Vector operator/(const Vector& U, const Vector& V);
  friend Vector operator/=(const Vector& U, const Vector& V);
  friend Vector operator+(const Vector& U, const Vector& V);
  friend Vector operator-(const Vector& U, const Vector& V);
  friend Vector operator-(const Vector& U);
  friend Vector operator+=(const Vector& U, const Vector& V);
  friend Vector operator-=(const Vector& U, const Vector& V);

  friend std::ostream& operator<<(std::ostream& out, const Vector& V);

  ScalarType X, Y, Z;
};
} // namespace GAM