#pragma once 

#include "geometry.h"

namespace GAM 
{
class TMesh 
{
public:
  TMesh()=default; 
  TMesh(const std::vector<ScalarType>& values) : m_Values(values) {} 

  inline void SetVerticesValues(const std::vector<ScalarType>& values) { m_Values= values; }
  inline std::vector<ScalarType> GetVerticesValues(const std::vector<ScalarType>& values) const { return m_Values; }
  inline ScalarType GetVertexValue(IndexType iVertex) const { return m_Values[iVertex]; }

  /// @brief Get the number of vertex of the mesh. 
  /// @return the number of vertex.
  inline IndexType GetNumberOfVertex() const { return m_Vertices.size(); }
  
  /// @brief Get the number of face of the mesh.
  /// @return the number of face.
  inline IndexType GetNumberOfFace() const { return m_Faces.size(); }

  /// @brief Load and triangulate a mesh from an OFF file. 
  /// @param OFFFile the path of an .off file. 
  void LoadOFF(const std::string& OFFFile);

  /// @brief Save the mesh as an .obj file. 
  /// @param OBJFile name of the .obj file.  
  void SaveOBJ(const std::string& OBJFile);

  /// @brief Get the local index for a vertex located on the face of index `iFace`.
  /// @param iVertex index of the vertex.
  /// @param iFace index of the face.
  /// @return the local index.
  IndexType GetVertexLocalIndex(IndexType iVertex, IndexType iFace) const;  
  
  /// @brief Print the index of the neighboring faces of the face of index `iFace`. 
  /// @param iFace index of the face.
  void PrintNeighboringFacesOfFace(IndexType iFace) const;

  /// @brief Print the index of the neighboring faces of the vertex of index `iVertex`. 
  /// @param iVertex index of the vertex.
  void PrintNeighboringFacesOfVertex(IndexType iVertex) const;

  /// @brief Get the index of the neighboring faces of a face.
  /// @param iFace index of the face.  
  /// @return a vector of indices.
  std::vector<IndexType> GetNeighboringFacesOfFace(IndexType iFace) const;

  /// @brief Get the index of the neighboring faces of a vertex.
  /// @param iVertex index of the vertex.
  /// @return a vector of indices.
  std::vector<IndexType> GetNeighboringFacesOfVertex(IndexType iVertex) const;

  /// @brief Get the index of the neighboring vertices of a vertex.
  /// @param iVertex index of the vertex.
  /// @return a vector of indices.
  std::vector<IndexType> GetNeighboringVerticesOfVertex(IndexType iVertex) const;

  /// @brief Calculate the area of the face of index iFace.
  /// @param iFace index of the face.
  /// @return a scalar corresponding to the area of the face.
  ScalarType FaceArea(IndexType iFace) const;

  /// @brief Calculate the area of the patch of surface corresponding to the vertex of index iVertex.
  /// @param iVertex index of the vertex. 
  /// @return a scalar corresponding to the area of the patch.
  ScalarType PatchAreaForVertex(IndexType iVertex) const;

  /// @brief Calculate the Laplacian of a discrete function defined on the mesh.
  /// @return An array of Laplacian values for each vertex.  
  void CotangentLaplacian();

  /// @brief Compute normal of each vertex of the mesh. 
  void ComputeNormals();

  void ComputeCurvature();

  void HeatDiffusion(IndexType iVertex, ScalarType value);

private: 
  /// @brief Calculate cotangente Laplacian value at vertex of index iVertex.
  /// @param iVertex index of the vertex.
  ScalarType CotangentLaplacian(IndexType iVertex);

  /// @brief Calculate the normal of a vertex using the cotangent Laplacian. 
  /// @param iVertex index of the vertex.
  /// @return the normal of the vertex.
  Vector VertexNormal(IndexType iVertex);

  /// @brief Checking the integrity of the mesh structure.
  void IntegrityCheck() const; 

private: 
  /// @brief Vertices of the mesh.
  std::vector<Vertex> m_Vertices;

  /// @brief Sewn-together faces of the mesh. 
  std::vector<Face> m_Faces;

  /// @brief Vertices normales (must be of the same size as m_Vertices).
  std::vector<Vector> m_Normals;

  /// @brief Vertices values (must be of the same size as m_Vertices). 
  std::vector<ScalarType> m_Values;

  BoundingBox m_Bbox;

  ScalarType m_MinCurv; 
  ScalarType m_MaxCurv; 
};
} // namespace GAM