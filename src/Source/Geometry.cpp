#include "Geometry.h"

namespace gam
{

    ScalarType cotan(const Vector &u, const Vector &v)
    {
        ScalarType sinTheta = length(cross(u, v));
        if (sinTheta > 0. || sinTheta < 0.)
        {
            ScalarType cosTheta = dot(u, v);
            return cosTheta / sinTheta;
        }

        return 0.;
    }

    int orientation(const Point &p, const Point &q, const Point &r)
    {
        ScalarType s = (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - p.x);

        return s > 0.0 ? 1 : s < 0.0 ? -1
                                     : 0;
    }

    std::pair<bool, int> in_triangle(const Vertex &p, const Vertex &a, const Vertex &b, const Vertex &c)
    {
        int d1 = orientation(Vertex::as_point(p), Vertex::as_point(a), Vertex::as_point(b));
        int d2 = orientation(Vertex::as_point(p), Vertex::as_point(b), Vertex::as_point(c));
        int d3 = orientation(Vertex::as_point(p), Vertex::as_point(c), Vertex::as_point(a));

        if (d1 == 0 && d2 == 1 && d3 == 1)
            return {true, 2};
        if (d1 == 1 && d2 == 0 && d3 == 1)
            return {true, 0};
        if (d1 == 1 && d2 == 1 && d3 == 0)
            return {true, 1};

        if (d1 == 1 && d2 == 1 && d3 == 1)
            return {true, -1};

        return {false, -1};
    }

    bool intersect(const Point &a, const Point &b, const Point &c, const Point &d)
    {
        return (orientation(c, a, b) * orientation(d, a, b) < 0) && (orientation(a, d, c) * orientation(b, d, c) < 0);
    }

    int intersected_edge(const Point &a, const Point &b, const Point &c, const Point &d, const Point &e)
    {
        return intersect(a, b, c, d) ? 2 : intersect(a, b, e, c) ? 1
                                       : intersect(a, b, d, e)   ? 0
                                                                 : -1;
    }

    bool in_circle(const Point &p, const Point &a, const Point &b, const Point &c)
    {
        ScalarType c00 = b.x - a.x;
        ScalarType c01 = c.x - a.x;
        ScalarType c02 = p.x - a.x;

        ScalarType c10 = b.y - a.y;
        ScalarType c11 = c.y - a.y;
        ScalarType c12 = p.y - a.y;

        ScalarType c20 = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
        ScalarType c21 = (c.x - a.x) * (c.x - a.x) + (c.y - a.y) * (c.y - a.y);
        ScalarType c22 = (p.x - a.x) * (p.x - a.x) + (p.y - a.y) * (p.y - a.y);

        ScalarType d = c00 * det(c11, c12, c21, c22) - c01 * det(c10, c12, c20, c22) + c02 * det(c10, c11, c20, c21);

        return d <= 0;
    }

    ScalarType det(ScalarType i, ScalarType j, ScalarType k, ScalarType l)
    {
        return i * l - j * k;
    }

    Vertex::Vertex(const Point &point, int i_face) : X(point.x), Y(point.y), Z(point.z), FaceIndex(i_face)
    {
    }

    Vertex::Vertex(ScalarType x, ScalarType y, ScalarType z, int i_face) : X(x), Y(y), Z(z), FaceIndex(i_face)
    {
    }

    std::ostream &operator<<(std::ostream &out, const Vertex &V)
    {
        out << "Vertex : [" << V.X << ", " << V.Y << ", " << V.Z << "]";
        return out;
    }

    Face::Face(int v0, int v1, int v2, int n0, int n1, int n2)
    {
        vertices(v0, v1, v2);
        neighbors(n0, n1, n2);
    }

    void Face::vertices(int v0, int v1, int v2)
    {
        Vertices[0] = v0;
        Vertices[1] = v1;
        Vertices[2] = v2;
    }

    void Face::neighbors(int n0, int n1, int n2)
    {
        Neighbors[0] = n0;
        Neighbors[1] = n1;
        Neighbors[2] = n2;
    }

    int &Face::operator()(int i)
    {
        assert(i >= 0 && i < 3);
        return Neighbors[i];
    }

    int Face::operator()(int i) const
    {
        assert(i >= 0 && i < 3);
        return Neighbors[i];
    }

    int &Face::operator[](int i)
    {
        assert(i >= 0 && i < 3);
        return Vertices[i];
    }

    void Face::slide_vertices_left()
    {
        auto tmp = Vertices[0];
        Vertices[0] = Vertices[1];
        Vertices[1] = Vertices[2];
        Vertices[2] = tmp;

        tmp = Neighbors[0];
        Neighbors[0] = Neighbors[1];
        Neighbors[1] = Neighbors[2];
        Neighbors[2] = tmp;
    }

    void Face::slide_vertices_right()
    {
        auto tmp = Vertices[0];
        Vertices[0] = Vertices[2];
        Vertices[1] = tmp;
        Vertices[2] = Vertices[1];

        tmp = Neighbors[0];
        Neighbors[0] = Neighbors[2];
        Neighbors[1] = tmp;
        Neighbors[2] = Neighbors[1];
    }

    void Face::change_neighbor(IndexType i_face, IndexType value)
    {
        for (auto &n : Neighbors)
        {
            if (n == i_face)
            {
                n = value;
                break;
            }
        }
    }

    int Face::get_edge(IndexType i_face) const
    {
        for (int i = 0; i < 3; ++i)
        {
            if (Neighbors[i] == i_face)
                return i;
        }

        return -1;
    }

    int Face::operator[](int i) const
    {
        assert(i >= 0 && i < 3);
        return Vertices[i];
    }

} // namespace gam