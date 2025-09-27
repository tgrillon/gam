#pragma once

#include "Utils.h"

namespace gam
{
// Forward declaration
struct Face;

struct Vertex
{
	Vertex() = default;
	Vertex(const Point& point, int i_face = -1);
	Vertex(scalar_t x, scalar_t y, scalar_t z, int i_face = -1);

	inline Point& as_point() { return reinterpret_cast<Point&>(*this); }

	inline const Point& as_point() const { return reinterpret_cast<const Point&>(*this); }

	inline Vector& as_vector() { return reinterpret_cast<Vector&>(*this); }

	inline const Vector& as_vector() const { return reinterpret_cast<const Vector&>(*this); }

	friend std::ostream& operator<<(std::ostream& out, const Vertex& v);

	// Attribute members
	scalar_t X{ 0 }, Y{ 0 }, Z{ 0 };
	int FaceIndex{ -1 };
};

struct Face
{
	Face() = default;
	Face(int v0, int v1, int v2, int n0 = -1, int n1 = -1, int n2 = -1);

	void set_vertex_indices(const int32_t v0, const int32_t v1, const int32_t v2);
	void set_neighbor_indices(const int32_t n0, const int32_t n1, const int32_t n2);

	//! Get neighbor of index i.
	int32_t operator()(const uint8_t i) const;
	int32_t& operator()(const uint8_t i);

	//! Get vertex of index i.
	int32_t operator[](const uint8_t i) const;
	int32_t& operator[](const uint8_t i);

	void slide_vertices_left();
	void slide_vertices_right();

	void change_neighbor(const index_t i_face, const index_t i_neighbor);

	int get_edge(const index_t i_face) const;

	int32_t VertexIndices[3]{ -1, -1, -1 };
	int32_t NeighborIndices[3]{ -1, -1, -1 };
};

scalar_t cotan(const Vector& u, const Vector& v);

//! Orientation predicate : returns a positive value if the three given points are oriented counter-clockwise, negative if they are oriented clockwise or zero if they are aligned.
int orientation(const Point& a, const Point& b, const Point& c);

//! In triangle predicate : returns a positive value if p is located inside the triangle defined by the vertices (a, b, c), negative if it is located outside or zero if it is located on the boundary.
std::pair<bool, int> in_triangle(const Vertex& p, const Vertex& a, const Vertex& b, const Vertex& c);

//! Segments intersection predicate : returns true if the segment AB intersects with the segment CD, false if not.
bool intersect(const Point& a, const Point& b, const Point& c, const Point& d);

//! Returns the index of the edge of the CDE triangle intersected by the segment AB, -1 if no intersection exists.
int intersected_edge(const Point& a, const Point& b, const Point& c, const Point& d, const Point& e);

//! Returns true if a point p is in a circle circumscribed at a, b and c, false otherwise.
bool in_circle(const Point& p, const Point& a, const Point& b, const Point& c);

scalar_t det(scalar_t i, scalar_t j, scalar_t k, scalar_t l);

} // namespace gam
