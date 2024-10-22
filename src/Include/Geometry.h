#pragma once

#include "Utils.h"

namespace gam
{
    struct Vertex
    {
        Vertex() = default;
        Vertex(const Point &point, int i_face = -1);
        Vertex(ScalarType x, ScalarType y, ScalarType z, int i_face = -1);

        ScalarType X{0}, Y{0}, Z{0};
        int FaceIndex{-1};

        //! Transform a Vertex into a Point.
        inline static Point as_point(const Vertex &v) { return Point(v.X, v.Y, v.Z); }
        inline static Vector as_vector(const Vertex &v) { return Vector(v.X, v.Y, v.Z); }

        friend std::ostream &operator<<(std::ostream &out, const Vertex &v);
    };

    struct Face
    {
        Face() = default;
        Face(int v0, int v1, int v2, int n0 = -1, int n1 = -1, int n2 = -1);

        void vertices(int v0, int v1, int v2);
        void neighbors(int n0, int n1, int n2);

        //! Get neighbor of index i.
        int operator()(int i) const;
        //! Get neighbor of index i.
        int &operator()(int i);

        //! Get vertex of index i.
        int operator[](int i) const;
        //! Get vertex of index i.
        int &operator[](int i);

        void slide_vertices_left();
        void slide_vertices_right();

        void change_neighbor(IndexType i_face, IndexType value);

        int get_edge(IndexType i_face) const;

        int Vertices[3] {-1, -1, -1};
        int Neighbors[3]{-1, -1, -1};
    };

    ScalarType cotan(const Vector &u, const Vector &v);

    //! Orientation predicate : returns a positive value if the three given points are oriented counter-clockwise, negative if they are oriented clockwise or zero if they are aligned.
    int orientation(const Point &a, const Point &b, const Point &c);

    //! In triangle predicate : returns a positive value if p is located inside the triangle defined by the vertices (a, b, c), negative if it is located outside or zero if it is located on the boundary.
    std::pair<bool, int> in_triangle(const Vertex &p, const Vertex &a, const Vertex &b, const Vertex &c);

    //! Segments intersection predicate : returns true if the segment AB intersects with the segment CD, false if not. 
    bool intersect(const Point& a, const Point& b, const Point& c, const Point& d);

    //! Returns the index of the edge of the CDE triangle intersected by the segment AB, -1 if no intersection exists.  
    int intersected_edge(const Point& a, const Point& b, const Point& c, const Point& d, const Point& e);

    //! Returns true if a point p is in a circle circumscribed at a, b and c, false otherwise.
    bool in_circle(const Point& p, const Point& a, const Point& b, const Point& c);

    ScalarType det(ScalarType i, ScalarType j, ScalarType k, ScalarType l);

} // namespace gam