#pragma once 

#include "geometry.h"

namespace GAM 
{
/// @brief Triangulated mesh. 
class TMesh 
{
public:
  TMesh()=default; 
  TMesh(const std::vector<ScalarType>& values) : m_Values(values) {} 

  //! Set the vertex value at index i.
  inline void vertex_value(IndexType iVertex, ScalarType v) { assert(iVertex < number_of_vertices()); m_Values[iVertex]= v; }

  //! Set the values of all vertices.
  inline void vertices_values(const std::vector<ScalarType>& values) { m_Values= values; }

  //! Set vertices values to 0.
  inline void reset_values() { m_Values= std::vector<ScalarType>(number_of_vertices(), 0.); }

  //! Get the vertex value at index i.
  inline ScalarType vertex_value(IndexType iVertex) const { assert(iVertex < number_of_vertices()); return m_Values[iVertex]; }

  //! Get the values of all vertices.
  inline std::vector<ScalarType> vertices_values() const { return m_Values; }

  //! Get the number of vertex of the mesh. 
  inline IndexType number_of_vertices() const { return m_Vertices.size(); }
  
  //! Get the number of face of the mesh.
  inline IndexType  number_of_faces() const { return m_Faces.size(); }

  //! Load and triangulate a mesh from an OFF file. 
  void load_off(const std::string& OFFFile);

  //! Save the mesh as an .obj file. 
  void save_obj(const std::string& OBJFile, bool useCurvature= false);

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

  Vector face_normal(IndexType iFace) const;

  //! Compute normal of each vertex of the mesh. 
  void smooth_normals();

  //! Compute curvature value at each vertex. 
  void curvature();

  //! Perform heat diffusion using the Laplacian equation. 
  void heat_diffusion(ScalarType deltaTime);

  //! Calculate the vertex value for the heat diffusion. 
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
  std::vector<Vertex> m_Vertices;

  //! Sewn-together faces of the mesh. 
  std::vector<Face> m_Faces;

  //! Vertices normales (must be of the same size as m_Vertices).
  std::vector<Vector> m_Normals;

  //! Vertices values (must be of the same size as m_Vertices). 
  std::vector<ScalarType> m_Values;

  //! Vertices curavture (must be of the same size as m_Vertices). 
  std::vector<ScalarType> m_Curvature;
};
} // namespace GAM