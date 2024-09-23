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

  inline void vertex_value(IndexType iVertex, ScalarType v) { m_Values[iVertex]= v; }
  inline void vertices_values(const std::vector<ScalarType>& values) { m_Values= values; }
  inline ScalarType vertex_value(IndexType iVertex) const { return m_Values[iVertex]; }
  inline std::vector<ScalarType> vertices_values(const std::vector<ScalarType>& values) const { return m_Values; }

  /// @brief Get the number of vertex of the mesh. 
  /// @return the number of vertex.
  inline IndexType number_of_vertices() const { return m_Vertices.size(); }
  
  /// @brief Get the number of face of the mesh.
  /// @return the number of face.
  inline IndexType  number_of_faces() const { return m_Faces.size(); }

  /// @brief Load and triangulate a mesh from an OFF file. 
  /// @param OFFFile the path of an .off file. 
  void load_off(const std::string& OFFFile);

  /// @brief Save the mesh as an .obj file. 
  /// @param OBJFile name of the .obj file.  
  void save_obj(const std::string& OBJFile);

  /// @brief Get the local index for a vertex located on the face of index `iFace`.
  /// @param iVertex index of the vertex.
  /// @param iFace index of the face.
  /// @return the local index.
  IndexType local_index(IndexType iVertex, IndexType iFace) const;  
  
  /// @brief Print the index of the neighboring faces of the face of index `iFace`. 
  /// @param iFace index of the face.
  void print_neighboring_faces_of_face(IndexType iFace) const;

  /// @brief Print the index of the neighboring faces of the vertex of index `iVertex`. 
  /// @param iVertex index of the vertex.
  void print_neighboring_faces_of_vertex(IndexType iVertex) const;

  /// @brief Get the index of the neighboring faces of a face.
  /// @param iFace index of the face.  
  /// @return a vector of indices.
  std::vector<IndexType> neighboring_faces_of_face(IndexType iFace) const;

  /// @brief Get the index of the neighboring faces of a vertex.
  /// @param iVertex index of the vertex.
  /// @return a vector of indices.
  std::vector<IndexType> neighboring_faces_of_vertex(IndexType iVertex) const;

  /// @brief Get the index of the neighboring vertices of a vertex.
  /// @param iVertex index of the vertex.
  /// @return a vector of indices.
  std::vector<IndexType> neighboring_vertices_of_vertex(IndexType iVertex) const;

  /// @brief Calculate the area of the face of index iFace.
  /// @param iFace index of the face.
  /// @return a scalar corresponding to the area of the face.
  ScalarType face_area(IndexType iFace) const;

  /// @brief Calculate the area of the patch of surface corresponding to the vertex of index iVertex.
  /// @param iVertex index of the vertex. 
  /// @return a scalar corresponding to the area of the patch.
  ScalarType patch_area(IndexType iVertex) const;

  /// @brief Calculate the Laplacian of a discrete function defined on the mesh.
  /// @return An array of Laplacian values for each vertex.  
  void laplacian();

  Vector face_normal(IndexType iFace) const;

  /// @brief Compute normal of each vertex of the mesh. 
  void smooth_normals();

  void curvature();

  void heat_diffusion(ScalarType deltaTime, IndexType iVertex0, IndexType iVertex1);
  void heat_diffusion(IndexType iVertex, ScalarType deltaTime);

private: 
  /// @brief Calculate cotangente Laplacian value at vertex of index iVertex.
  /// @param iVertex index of the vertex.
  ScalarType laplacian(IndexType iVertex);

  /// @brief Calculate the normal of a vertex using the cotangent Laplacian. 
  /// @param iVertex index of the vertex.
  /// @return the normal of the vertex.
  Vector laplacian_vector(IndexType iVertex);

  /// @brief Checking the integrity of the mesh structure.
  void integrity_check() const; 

private: 
  /// @brief Vertices of the mesh.
  std::vector<Vertex> m_Vertices;

  /// @brief Sewn-together faces of the mesh. 
  std::vector<Face> m_Faces;

  /// @brief Vertices normales (must be of the same size as m_Vertices).
  std::vector<Vector> m_Normals;

  /// @brief Vertices values (must be of the same size as m_Vertices). 
  std::vector<ScalarType> m_Values;
};
} // namespace GAM