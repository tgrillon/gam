#include "Geometry.h"

#include "PCH/PCH.h"

#include "Utils.h"

namespace gam
{
scalar_t cotan(const Vector& u, const Vector& v)
{
	scalar_t sinTheta = length(cross(u, v));
	if(sinTheta > 0. || sinTheta < 0.)
	{
		scalar_t cosTheta = dot(u, v);
		return cosTheta / sinTheta;
	}

	return 0.;
}

int orientation(const Point& p, const Point& q, const Point& r)
{
	scalar_t s = (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - p.x);

	return s > 0.0 ? 1 : s < 0.0 ? -1 : 0;
}

std::pair<bool, int> in_triangle(const Vertex& p, const Vertex& a, const Vertex& b, const Vertex& c)
{
	int d1 = orientation(p.as_point(), a.as_point(), b.as_point());
	int d2 = orientation(p.as_point(), b.as_point(), c.as_point());
	int d3 = orientation(p.as_point(), c.as_point(), a.as_point());

	if(d1 == 0 && d2 == 1 && d3 == 1)
		return { true, 2 };
	if(d1 == 1 && d2 == 0 && d3 == 1)
		return { true, 0 };
	if(d1 == 1 && d2 == 1 && d3 == 0)
		return { true, 1 };

	if(d1 == 1 && d2 == 1 && d3 == 1)
		return { true, -1 };

	return { false, -1 };
}

bool intersect(const Point& a, const Point& b, const Point& c, const Point& d)
{
	return (orientation(c, a, b) * orientation(d, a, b) < 0) && (orientation(a, d, c) * orientation(b, d, c) < 0);
}

int intersected_edge(const Point& a, const Point& b, const Point& c, const Point& d, const Point& e)
{
	return intersect(a, b, c, d) ? 2 : intersect(a, b, e, c) ? 1 : intersect(a, b, d, e) ? 0 : -1;
}

bool in_circle(const Point& p, const Point& a, const Point& b, const Point& c)
{
	scalar_t c00 = b.x - a.x;
	scalar_t c01 = c.x - a.x;
	scalar_t c02 = p.x - a.x;

	scalar_t c10 = b.y - a.y;
	scalar_t c11 = c.y - a.y;
	scalar_t c12 = p.y - a.y;

	scalar_t c20 = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
	scalar_t c21 = (c.x - a.x) * (c.x - a.x) + (c.y - a.y) * (c.y - a.y);
	scalar_t c22 = (p.x - a.x) * (p.x - a.x) + (p.y - a.y) * (p.y - a.y);

	scalar_t d = c00 * det(c11, c12, c21, c22) - c01 * det(c10, c12, c20, c22) + c02 * det(c10, c11, c20, c21);

	return d <= 0;
}

scalar_t det(scalar_t i, scalar_t j, scalar_t k, scalar_t l)
{
	return i * l - j * k;
}

Vertex::Vertex(const Point& point, int i_face)
	: X(point.x)
	, Y(point.y)
	, Z(point.z)
	, FaceIndex(i_face)
{}

Vertex::Vertex(scalar_t x, scalar_t y, scalar_t z, int i_face)
	: X(x)
	, Y(y)
	, Z(z)
	, FaceIndex(i_face)
{}

std::ostream& operator<<(std::ostream& out, const Vertex& V)
{
	out << "Vertex : [" << V.X << ", " << V.Y << ", " << V.Z << "]";
	return out;
}

Face::Face(int v0, int v1, int v2, int n0, int n1, int n2)
{
	set_vertex_indices(v0, v1, v2);
	set_neighbor_indices(n0, n1, n2);
}

void Face::set_vertex_indices(int32_t v0, int32_t v1, int32_t v2)
{
	VertexIndices[0] = v0;
	VertexIndices[1] = v1;
	VertexIndices[2] = v2;
}

void Face::set_neighbor_indices(int32_t n0, int32_t n1, int32_t n2)
{
	NeighborIndices[0] = n0;
	NeighborIndices[1] = n1;
	NeighborIndices[2] = n2;
}

int& Face::operator()(uint8_t i)
{
	assert(i >= 0 && i < 3);
	return NeighborIndices[i];
}

int Face::operator()(uint8_t i) const
{
	assert(i >= 0 && i < 3);
	return NeighborIndices[i];
}

int& Face::operator[](uint8_t i)
{
	assert(i >= 0 && i < 3);
	return VertexIndices[i];
}

void Face::slide_vertices_left()
{
	auto tmp = VertexIndices[0];
	VertexIndices[0] = VertexIndices[1];
	VertexIndices[1] = VertexIndices[2];
	VertexIndices[2] = tmp;

	tmp = NeighborIndices[0];
	NeighborIndices[0] = NeighborIndices[1];
	NeighborIndices[1] = NeighborIndices[2];
	NeighborIndices[2] = tmp;
}

void Face::slide_vertices_right()
{
	auto tmp = VertexIndices[0];
	VertexIndices[0] = VertexIndices[2];
	VertexIndices[1] = tmp;
	VertexIndices[2] = VertexIndices[1];

	tmp = NeighborIndices[0];
	NeighborIndices[0] = NeighborIndices[2];
	NeighborIndices[1] = tmp;
	NeighborIndices[2] = NeighborIndices[1];
}

void Face::change_neighbor(index_t i_face, index_t value)
{
	for(auto& n : NeighborIndices)
	{
		if(n == static_cast<int32_t>(i_face))
		{
			n = value;
			break;
		}
	}
}

int Face::get_edge(index_t i_face) const
{
	for(int i = 0; i < 3; ++i)
	{
		if(NeighborIndices[i] == static_cast<int32_t>(i_face))
			return i;
	}

	return -1;
}

int Face::operator[](uint8_t i) const
{
	assert(i >= 0 && i < 3);
	return VertexIndices[i];
}
} // namespace gam
