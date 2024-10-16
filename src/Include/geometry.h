#pragma once

#include "utils.h"

namespace gam
{
struct Point
{
  ScalarType X, Y, Z; 

  Point();
  Point(ScalarType x, ScalarType y, ScalarType z);

  //! Get the point with the mininmum value between a and b for each x, y and z component.
  static Point pmin(const Point& a, const Point& b); 

  //! Get the point with the maximum value between a and b for each x, y and z component.
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

  friend std::ostream& operator<<(std::ostream& out, const Point& p);
};

struct Vertex 
{
  Vertex()=default;
  Vertex(const Point& point, int iFace=-1);

  ScalarType X {0}, Y {0}, Z {0}; 
  int FaceIndex {-1}; 

  //! Transform a Vertex into a Point.
  inline static Point as_point(const Vertex& V) { return Point(V.X, V.Y, V.Z); }

  friend std::ostream& operator<<(std::ostream& out, const Vertex& V);
};

struct Face
{
  Face()=default; 
  Face(int v0, int v1, int v2, int n0, int n1, int n2);

  void vertices(int v0, int v1, int v2);
  void neighbors(int n0, int n1, int n2);

  //! Get neighbor of index i.
  int operator()(int i) const;
  //! Get neighbor of index i.
  int& operator()(int i);

  //! Get vertex of index i.
  int operator[](int i) const;
  //! Get vertex of index i.
  int& operator[](int i);

  void change_neighbor(IndexType iFace, IndexType value);

  int get_edge(IndexType iFace);

  int Vertices[3]  { -1, -1, -1 };
  int Neighbors[3] { -1, -1, -1 };
};

struct Vector 
{
  //! Get the unit vector of x-axis. 
  inline static Vector unit_x() { return {1., 0., 0.}; }
  //! Get the unit vector of y-axis. 
  inline static Vector unit_y() { return {0., 1., 0.}; }
  //! Get the unit vector of z-axis. 
  inline static Vector unit_z() { return {0., 0., 1.}; }
  //! Get the null vector. 
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

  //! Calculate the dot product between the vector and V.
  inline ScalarType dot(const Vector& V) const { return X*V.X+Y*V.Y+Z*V.Z; }

  //! Calculate the cross product between the vector and V.
  inline Vector cross(const Vector& V) const { return Vector(Y*V.Z-Z*V.Y, Z*V.X-X*V.Z, X*V.Y-Y*V.X); }

  //! Calculate the magnitude²/norm² of the vector.
  inline ScalarType norm2() const { return X*X+Y*Y+Z*Z; };

  //! Calculate the magnitude/norm of the vector.
  inline ScalarType norm() const { return std::sqrt(norm2()); }

  //! Normalize the vector.
  inline void normalize() { (*this)= this->normalized(); }

  //! Return a normalize vector.
  inline Vector normalized() const { Vector n(X, Y, Z); return n/n.norm(); }

  //! Calculate the cotangent between the vector and V.
  ScalarType cotan(const Vector& V) const;

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

//! Orientation predicate : returns a positive value if the three given points are oriented counter-clockwise, negative if they are oriented clockwise or zero if they are aligned.
int orientation(const Point& a, const Point& b, const Point& c);

//! Returns a positive value if p is located inside the triangle defined by the vertices (a, b, c), negative if it is located outside or zero if it is located on the boundary. 
std::pair<bool, int> in_triangle(const Vertex& p, const Vertex& a, const Vertex& b, const Vertex& c);
} // namespace gam