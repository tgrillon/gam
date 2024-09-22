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
  static Point Min(const Point& a, const Point& b); 

  /// @brief Get the point with the maximum value between a and b for each x, y and z component.
  /// @return the point with the maximum value between a and b for each component. 
  static Point Max(const Point& a, const Point& b); 

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

class BoundingBox
{
public:
  BoundingBox();
  BoundingBox(const Point& min, const Point& max); 
  
  /// @brief Get the center of the bounding box. 
  /// @return a point corresponding to the center of the bounding box. 
  inline Point Center() const { return 0.5 * (m_Pmin + m_Pmax); }

  /// @brief Get the attribute Pmin of the bounding box.
  /// @return Pmin.
  inline Point GetMin() const { return m_Pmin; }

  inline Point& Min() { return m_Pmin; }

  /// @brief Get the attribute Pmax of the bounding box.
  /// @return Pmax.
  inline Point GetMax() const { return m_Pmax; }

  inline Point& Max() { return m_Pmax; }

private: 
  Point m_Pmin, m_Pmax;   
};

struct Vertex 
{
  ScalarType X, Y, Z; 
  int FaceIndex { -1 }; 

  inline static Point AsPoint(const Vertex& V) { return Point(V.X, V.Y, V.Z); }

  friend std::ostream& operator<<(std::ostream& out, const Vertex& V);
};

struct Face
{
  int Vertices[3]  { -1, -1, -1 };
  int Neighbors[3] { -1, -1, -1 };
};

struct Vector 
{
  inline static Vector UnitX() { return {1., 0., 0.}; }
  inline static Vector UnitY() { return {0., 1., 0.}; }
  inline static Vector UnitZ() { return {0., 0., 1.}; }
  inline static Vector Null()  { return {0., 0., 0.}; }

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
  inline ScalarType Dot(const Vector& V) const { return X*V.X+Y*V.Y+Z*V.Z; }

  /// @brief Calculate the cross product between `this` and V.
  /// @param V a vector.
  /// @return the orthogonal vector to `this` and V.
  inline Vector Cross(const Vector& V) const { return Vector(Y*V.Z-Z*V.Y, Z*V.X-X*V.Z, X*V.Y-Y*V.X); }

  /// @brief Calculate the magnitude²/norm² of `this`.
  /// @return the norm² of `this`.
  inline ScalarType Norm2() const { return X*X+Y*Y+Z*Z; };

  /// @brief Calculate the magnitude/norm of `this`.
  /// @return the norm of `this`.
  inline ScalarType Norm() const { return std::sqrt(Norm2()); }

  /// @brief Normalize `this`.
  inline void Normalize() { (*this)= this->Normalized(); }

  /// @brief Return a normalize vector.
  /// @return the normalized vector of `this`. 
  inline Vector Normalized() const { return (*this)/this->Norm(); }

  /// @brief Calculate the cotangent between `this` and V.
  /// @param V a vector.
  /// @return the value of cot(theta) with theta the angle between `this` and V.
  inline ScalarType Cotan(const Vector& V) const { return this->Dot(V)/(this->Cross(V)).Norm(); }

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