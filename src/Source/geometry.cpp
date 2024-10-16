#include "geometry.h"

namespace gam
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

int orientation(const Point &p, const Point &q, const Point &r)
{
    ScalarType s = (q.X - p.X) * (r.Y - p.Y) - (q.Y - p.Y) * (r.X - p.X);

    return s > 0.0 ? 1 : s < 0.0 ? -1 : 0;
}

std::pair<bool, int> in_triangle(const Vertex &p, const Vertex &a, const Vertex &b, const Vertex &c)
{
    int d1 = orientation(Vertex::as_point(p), Vertex::as_point(a), Vertex::as_point(b));
    int d2 = orientation(Vertex::as_point(p), Vertex::as_point(b), Vertex::as_point(c));
    int d3 = orientation(Vertex::as_point(p), Vertex::as_point(c), Vertex::as_point(a));

    if (d1 == 0) return { true, 2 };
    if (d2 == 0) return { true, 0 };
    if (d3 == 0) return { true, 1 };

    if (d1 == 1 && d2 == 1 && d3 == 1) return { true, -1 }; 
    
    return { false, -1 }; 
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

std::ostream &operator<<(std::ostream &out, const Point &p)
{
    out << "Point : [" << p.X << ", " << p.Y << ", " << p.Z << "]"; 
    return out;
}

ScalarType Vector::cotan(const Vector &V) const
{
    ScalarType sinTheta= (this->cross(V)).norm();
    if (sinTheta > 0. || sinTheta < 0.)
    {
        ScalarType cosTheta= this->dot(V); 
        return cosTheta/sinTheta; 
    }

    return 0.;
}

Vertex::Vertex(const Point &point, int iFace) : X(point.X), Y(point.Y), Z(point.Z), FaceIndex(iFace)
{
}

Face::Face(int v0, int v1, int v2, int n0, int n1, int n2) 
{
    vertices(v0, v1, v2);
    neighbors(n0, n1, n2);
}

void Face::vertices(int v0, int v1, int v2)
{
    Vertices[0]= v0; 
    Vertices[1]= v1; 
    Vertices[2]= v2; 
}

void Face::neighbors(int n0, int n1, int n2)
{
    Neighbors[0]= n0; 
    Neighbors[1]= n1; 
    Neighbors[2]= n2; 
}

int& Face::operator()(int i) 
{
    assert(i >= 0 && i < 3);
    return Neighbors[i];
}

int Face::operator()(int i) const
{
    assert(i >= 0 && i < 3);
    return Neighbors[i];
}

int& Face::operator[](int i) 
{
    assert(i >= 0 && i < 3);
    return Vertices[i];
}

void Face::change_neighbor(IndexType iFace, IndexType value)
{
    for (auto& n : Neighbors)
    {
        if (n == iFace)
        {
            n = value; 
            break;
        }
    }
}

int Face::get_edge(IndexType iFace)
{
    for (int i = 0; i < 3; ++i)
    {
        if (Neighbors[i] == iFace) return i;
    }

    return -1;
}

int Face::operator[](int i) const
{
    assert(i >= 0 && i < 3);
    return Vertices[i];
}

} // namespace gam