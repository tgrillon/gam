#pragma once 

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cassert>
#include <utility>
#include <cmath>

#include "utils.hpp"

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

    friend std::ostream& operator<<(std::ostream& out, const Vertex& V);
  };

  struct Vector 
  {
    Vector(float x, float y, float z) : X(x), Y(y), Z(z) {}

    Vector(const Vertex& p, const Vertex& q)
    {	
      X= q.X-p.X;
      Y= q.Y-p.Y;
      Z= q.Z-p.Z;
    }

    inline float dot(const Vector& V) const { return X*V.X+Y*V.Y+Z*V.Z; }
    inline float norm2() const { return X*X+Y*Y+Z*Z; };
    inline float norm() const { return std::sqrt(norm2()); }

    inline Vector normalize() const { return (*this)/this->norm(); }

    inline Vector cross(const Vector& V) const { return Vector(Y*V.Z-Z*V.Y, Z*V.X-X*V.Z, X*V.Y-Y*V.X); }

    friend Vector operator*(float s, const Vector& V);
    friend Vector operator*(const Vector& V, float s);
    friend Vector operator/(const Vector& V, float d);

    friend std::ostream& operator<<(std::ostream& out, const Vector& V);

    float X, Y, Z;
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
    void LoadOFF(const std::string& OFFFile);

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

    /// @brief Calculate the area of the face of index iFace.
    /// @param iFace index of the face.
    /// @return a float corresponding to the area of the face.
    float CalculateFaceArea(size_t iFace) const;

    /// @brief Calculate the area of the patch of surface corresponding to the vertex of index iVertex.
    /// @param iVertex index of the vertex. 
    /// @return a float corresponding to the area of the patch.
    float CalculatePatchAreaForVertex(size_t iVertex) const;

  private: 
    /// @brief Checking the integrity of the mesh structure.
    void IntegrityCheck() const; 
  
  private: 
    std::vector<Vertex> m_Vertices;
    std::vector<Face> m_Faces;
  };

} // namespace GAM