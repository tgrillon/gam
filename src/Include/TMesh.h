#pragma once

#include "Geometry.h"

namespace gam
{
    //! Triangulated mesh.
    class TMesh
    {
    public:
        TMesh() = default;
        TMesh(const std::vector<ScalarType> &values) : m_values(values) {}

        Mesh mesh(bool curvature = true, bool remove_infinite = false) const;

        //! Set the vertex value at index i.
        inline void vertex_value(IndexType i_vertex, ScalarType v)
        {
            assert(i_vertex < vertex_count());
            m_values[i_vertex] = v;
        }

        //! Set the values of all vertices.
        inline void vertices_values(const std::vector<ScalarType> &values) { m_values = values; }

        //! Set vertices values to 0.
        inline void reset_values() { m_values = std::vector<ScalarType>(vertex_count(), 0.); }

        //! Get the vertex value at index i.
        inline ScalarType vertex_value(IndexType i_vertex) const
        {
            assert(i_vertex < vertex_count());
            return m_values[i_vertex];
        }

        //! Get the values of all vertices.
        inline std::vector<ScalarType> vertices_values() const { return m_values; }

        //! Get the number of vertex of the mesh.
        inline IndexType vertex_count() const { return m_vertices.size(); }

        //! Get the number of face of the mesh.
        inline IndexType face_count() const { return m_faces.size(); }

        //! Load and triangulate a mesh from an OFF file.
        void load_off(const std::string &off_file);

        //! Save the mesh as an .obj file.
        void save_obj(const std::string &obj_file, bool use_curvature = false);

        //! Get the local index for a vertex located on the face of index `i_face`.
        IndexType local_index(IndexType i_vertex, IndexType i_face) const;

        //! Print the index of the neighboring faces of the face of index `i_face`.
        void print_neighboring_faces_of_face(IndexType i_face) const;

        //! Print the index of the neighboring faces of the vertex of index `i_vertex`.
        void print_neighboring_faces_of_vertex(IndexType i_vertex) const;

        //! Get the index of the neighboring faces of a face.
        std::vector<IndexType> neighboring_faces_of_face(IndexType i_face) const;

        //! Get the index of the neighboring faces of a vertex.
        std::vector<IndexType> neighboring_faces_of_vertex(IndexType i_vertex) const;

        //! Get the index of the neighboring vertices of a vertex.
        std::vector<IndexType> neighboring_vertices_of_vertex(IndexType i_vertex) const;

        //! Calculate the area of the face of index i_face.
        ScalarType face_area(IndexType i_face) const;

        //! Calculate the area of the patch of surface corresponding to the vertex of index i_vertex.
        ScalarType patch_area(IndexType i_vertex) const;

        //! Calculate the Laplacian of a discrete function defined on the mesh.
        void laplacian();

        Vector face_normal(IndexType i_face) const;

        //! Compute normal of each vertex of the mesh.
        void smooth_normals();

        //! Compute curvature value at each vertex.
        void curvature();

        //! Perform heat diffusion using the Laplacian equation.
        void heat_diffusion(ScalarType delta_time);

        //! Calculate the vertex value for the heat diffusion.
        void heat_diffusion(IndexType i_vertex, ScalarType delta_time);

        //! Insert a vertex of position p.
        void insert_vertex(float x, float y, float z);

        //! Insert a vertex of position p.
        void insert_vertex(const Point &p);

        //! Flips the edge opposed to the vertex of local index i_edge within the face of index i_face.
        void flip_edge(IndexType i_face, IndexType i_edge);

        void insert_vertices(const std::vector<Point>& vertices, int point_count=-1);

        //! Clear the data structure.
        void clear();

    private:
        //! Calculate cotangente Laplacian value at vertex of index i_vertex.
        ScalarType laplacian(IndexType i_vertex);

        //! Calculate the normal of a vertex using the cotangent Laplacian.
        Vector laplacian_vector(IndexType i_vertex);

        //! Locate the triangle that contains p : <in_a_face (infinite face excluded), <face index, edge index>>
        std::pair<bool, std::pair<int, int>> locate_triangle(const Point& p) const;

        //! Insert a point that is outside the mesh.
        void insert_outside(const Point& p, IndexType i_face);

        //! Iterative delaunay triangulation
        void lawson(IndexType i_vertex);

        //! Returns true if i_face is an infinite faces, false otherwise. 
        bool is_infinite_face(IndexType i_face) const;

        //! Use for infinite faces, they must have the infinite point (of index 0) as first vertex (local index 0). This method check if the infinite face is well constructed, if not it do the necessary operation. 
        void slide_triangle(IndexType i_face);

        //! Splits a triangle face into three by insertion of a new vertex that is located at the position provided in parameter.
        void triangle_split(const Point &p, IndexType i_face);

        //! Splits an edge into two by insertion of a new vertex that is located at the position provided in parameter. The incident faces are also divided into two faces.
        void edge_split(const Point &p, IndexType i_face, IndexType i_edge);

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
        std::vector<ScalarType> m_values;

        //! Vertices curavture (must be of the same size as m_vertices).
        std::vector<ScalarType> m_curvature;
    };
} // namespace gam