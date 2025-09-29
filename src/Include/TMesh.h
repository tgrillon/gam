#pragma once

#include "Geometry.h"

#include <cassert>

namespace gam
{
//! Triangulated mesh.
class TMesh
{
public:
	TMesh() = default;

	TMesh(const std::vector<scalar_t>& values)
		: m_values(values)
	{}

	Mesh mesh(bool curvature = true, bool remove_infinite = false) const;

	//! Set a vertex position.
	void vertex(index_t i_vertex, const Point& p)
	{
		assert(i_vertex < vertex_count());
		m_vertices[i_vertex] = p;
	}

	//! Set the vertex value at index i.
	void vertex_value(index_t i_vertex, scalar_t v);

	//! Set the values of all vertices.
	inline void vertices_values(const std::vector<scalar_t>& values) { m_values = values; }

	//! Set vertices values to 0.
	inline void reset_values() { m_values = std::vector<scalar_t>(vertex_count(), 0.); }

	//! Get the vertex value at index i.
	scalar_t vertex_value(index_t i_vertex) const;

	//! Get the values of all vertices.
	inline std::vector<scalar_t> vertices_values() const { return m_values; }

	//! Get the number of vertex of the mesh.
	inline index_t vertex_count() const { return static_cast<index_t>(m_vertices.size()); }

	//! Get the number of face of the mesh.
	inline index_t face_count() const { return m_faces.size(); }

	//! Load and triangulate a mesh from an OFF file.
	void load_off(const std::string& off_file);

	//! Save the mesh as a .obj file.
	void save_obj(const std::string& obj_file, bool use_curvature = false, bool remove_inf = false);

	//! Save the mesh as a .off file.
	void save_off(const std::string& off_file, bool remove_inf = false);

	//! Get the local index for a vertex located on the face of index `i_face`.
	index_t local_index(index_t i_vertex, index_t i_face) const;

	//! Print the index of the neighboring faces of the face of index `i_face`.
	void print_neighboring_faces_of_face(index_t i_face) const;

	//! Print the index of the neighboring faces of the vertex of index `i_vertex`.
	void print_neighboring_faces_of_vertex(index_t i_vertex) const;

	//! Get the index of the neighboring faces of a face.
	std::vector<index_t> neighboring_faces_of_face(index_t i_face) const;

	//! Get the index of the neighboring faces of a vertex.
	std::vector<index_t> neighboring_faces_of_vertex(index_t i_vertex) const;

	//! Get the index of the neighboring vertices of a vertex.
	std::vector<index_t> neighboring_vertices_of_vertex(index_t i_vertex) const;

	//! Calculate the area of the face of index i_face.
	scalar_t face_area(index_t i_face) const;

	//! Calculate the area of the patch of surface corresponding to the vertex of index i_vertex.
	scalar_t patch_area(index_t i_vertex) const;

	//! Calculate the Laplacian of a discrete function defined on the mesh.
	void laplacian();

	Vector face_normal(index_t i_face) const;

	//! Compute normal of each vertex of the mesh.
	void smooth_normals();

	//! Compute curvature value at each vertex.
	void curvature();

	//! Perform heat diffusion using the Laplacian equation.
	void heat_diffusion(scalar_t delta_time);

	//! Calculate the vertex value for the heat diffusion.
	void heat_diffusion(index_t i_vertex, scalar_t delta_time);

	//! Insert a vertex of position p.
	void insert_vertex(float x, float y, float z);

	//! Insert a vertex of position p.
	void insert_vertex(const Point& p);

	//! Flips the edge opposed to the vertex of local index i_edge within the face of index i_face.
	void flip_edge(index_t i_face, index_t i_edge);

	void insert_vertices(const std::vector<Point>& vertices, int point_count = -1);

	//! Clear the data structure.
	void clear();

private:
	//! Calculate cotangente Laplacian value at vertex of index i_vertex.
	scalar_t laplacian(index_t i_vertex);

	//! Calculate the normal of a vertex using the cotangent Laplacian.
	Vector laplacian_vector(index_t i_vertex);

	//! Locate the triangle that contains p : <in_a_face (infinite face excluded), <face index, edge index>>
	std::pair<bool, std::pair<int32_t, int8_t>> locate_triangle(const Point& p) const;

	//! Insert a point that is outside the mesh.
	void insert_outside(const Point& p, index_t i_face);

	//! Iterative delaunay triangulation
	void lawson(index_t i_vertex);

	//! Returns true if i_face is an infinite faces, false otherwise.
	bool is_infinite_face(index_t i_face) const;
	bool is_infinite_face(Face face) const;

	//! Use for infinite faces, they must have the infinite point (of index 0) as first vertex (local index 0). This method check if the infinite face is well constructed, if not it do the necessary operation.
	void slide_triangle(index_t i_face);

	//! Splits a triangle face into three by insertion of a new vertex that is located at the position provided in parameter.
	void triangle_split(const Point& p, index_t i_face);

	//! Splits an edge into two by insertion of a new vertex that is located at the position provided in parameter. The incident faces are also divided into two faces.
	void edge_split(const Point& p, index_t i_face, index_t i_edge);

	//! Check if a face is well oriented (counter-clockwise), if not, it rearange the vertices.
	void check_orientation(Face& face);

	//! Checking the delaunay triangulation.
	void delaunay_check() const;

	//! Checking the integrity of the mesh structure.
	void integrity_check() const;

private:
	//! Vertices of the mesh.
	std::vector<Vertex> m_vertices;

	//! Sewn-together faces of the mesh.
	std::vector<Face> m_faces;

	//! Vertices normales (must be of the same size as m_vertices).
	std::vector<Vector> m_normals;

	//! Vertices values (must be of the same size as m_vertices).
	std::vector<scalar_t> m_values;

	//! Vertices curavture (must be of the same size as m_vertices).
	std::vector<scalar_t> m_curvature;
};
} // namespace gam
