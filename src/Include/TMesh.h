#pragma once 

#include "geometry.h"

namespace GAM 
{
//! Triangulated mesh. 
class TMesh 
{
public:
  TMesh()=default; 
  TMesh(const std::vector<ScalarType>& values) : m_values(values) {} 

  inline void vertex_value(IndexType iVertex, ScalarType v) { m_values[iVertex]= v; }
  inline void vertices_values(const std::vector<ScalarType>& values) { m_values= values; }
  inline ScalarType vertex_value(IndexType iVertex) const { return m_values[iVertex]; }
  inline std::vector<ScalarType> vertices_values(const std::vector<ScalarType>& values) const { return m_values; }

  //! Get the number of vertex of the mesh. 
  inline IndexType vertex_count() const { return m_vertices.size(); }
  
  //! Get the number of face of the mesh.
  inline IndexType face_count() const { return m_faces.size(); }

  //! Load and triangulate a mesh from an OFF file. 
  void load_off(const std::string& OFFFile);

  //! Save the mesh as an .obj file. 
  void save_obj(const std::string& OBJFile);

  //! Get the local index for a vertex located on the face of index `iFace`.
  IndexType local_index(IndexType iVertex, IndexType iFace) const;  
  
  //! Print the index of the neighboring faces of the face of index `iFace`. 
  void print_neighboring_faces_of_face(IndexType iFace) const;

  //! Print the index of the neighboring faces of the vertex of index `iVertex`. 
  void print_neighboring_faces_of_vertex(IndexType iVertex) const;

  //! Get the index of the neighboring faces of a face.
  std::vector<IndexType> neighboring_faces_of_face(IndexType iFace) const;

  //! Get the index of the neighboring faces of a vertex.
  std::vector<IndexType> neighboring_faces_of_vertex(IndexType iVertex) const;

  //! Get the index of the neighboring vertices of a vertex.
  std::vector<IndexType> neighboring_vertices_of_vertex(IndexType iVertex) const;

  //! Calculate the area of the face of index iFace.
  ScalarType face_area(IndexType iFace) const;

  //! Calculate the area of the patch of surface corresponding to the vertex of index iVertex.
  ScalarType patch_area(IndexType iVertex) const;

  //! Calculate the Laplacian of a discrete function defined on the mesh.
  void laplacian();

  //! Compute normal of the given face. 
  Vector face_normal(IndexType iFace) const;

  //! Compute normal of each vertex of the mesh. 
  void smooth_normals();

  //! Compute mesh curvature.
  void curvature();

  //! Heat diffusion operation on the whole mesh at a given time.
  void heat_diffusion(ScalarType deltaTime, IndexType iVertex0, IndexType iVertex1);

  //! Heat diffusion operation on the given vertex at a given time.
  void heat_diffusion(IndexType iVertex, ScalarType deltaTime);

private: 
  //! Calculate cotangente Laplacian value at vertex of index iVertex.
  ScalarType laplacian(IndexType iVertex);

  //! Calculate the normal of a vertex using the cotangent Laplacian. 
  Vector laplacian_vector(IndexType iVertex);

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
};
} // namespace GAM