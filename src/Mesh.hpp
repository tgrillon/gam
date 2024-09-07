#pragma once 

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cassert>
#include <utility>

namespace GAM 
{
  struct HashEdgePair
  {
    size_t operator()(const std::pair<int, int>& P) const 
    {
      return P.first + P.second; 
    }
  };

  struct Vertex 
  {
    float X, Y, Z; 
    int FaceIndex { -1 }; 
  };

  struct Face
  {
    int Vertices[3]  { -1, -1, -1 };
    int Neighbors[3] { -1, -1, -1 };
  };

  class Mesh 
  {
  public:
    Mesh()=default; 

    /// @brief Load and triangulate a mesh from an OFF file. 
    /// @param OFFFile the path of an .off file. 
    void Load(const std::string& OFFFile);

    /// @brief Get the local index for a vertex located on the face of index `iFace`.
    /// @param iVertex index of the vertex.
    /// @param iFace index of the face.
    /// @return the local index as an unsigned int.
    size_t GetVertexLocalIndex(size_t iVertex, size_t iFace) const;  
    
    /// @brief Print the index of the neighboring faces of the face of index `iFace`. 
    /// @param iFace index of the face.
    void PrintNeighboringFacesOfFace(size_t iFace) const;

    /// @brief Print the index of the neighboring faces of the vertex of index `iVertex`. 
    /// @param iVertex index of the vertex.
    void PrintNeighboringFacesOfVertex(size_t iVertex) const;

    /// @brief Get the index of the neighboring faces of a face.
    /// @param iFace index of the face.  
    /// @return a vector of indices as unsigned int.
    std::vector<size_t> GetNeighboringFacesOfFace(size_t iFace) const;

    /// @brief Get the index of the neighboring faces of a vertex.
    /// @param iVertex index of the vertex.
    /// @return a vector of indices as unsigned int.
    std::vector<size_t> GetNeighboringFacesOfVertex(size_t iVertex) const;

  private: 
    /// @brief Checking the integrity of the mesh structure.
    void IntegrityCheck() const; 
  
  private: 
    std::vector<Vertex> m_Vertices;
    std::vector<Face> m_Faces;
  };

} // namespace GAM