#pragma once 

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cassert>
#include <utility>
#include <cmath>
#include <ranges>

#include "utils.hpp"

namespace GAM 
{
  using ScalarType= float;
  using IndexType= size_t;

  struct HashEdgePair
  {
    IndexType operator()(const std::pair<IndexType, IndexType>& P) const 
    {
      return P.first + P.second; 
    }
  };

  struct Vertex 
  {
    ScalarType X, Y, Z; 
    int FaceIndex { -1 }; 

    friend std::ostream& operator<<(std::ostream& out, const Vertex& V);
  };

  struct Vector 
  {
    Vector(ScalarType x, ScalarType y, ScalarType z) : X(x), Y(y), Z(z) {}

    Vector(const Vertex& p, const Vertex& q)
    {	
      X= q.X-p.X;
      Y= q.Y-p.Y;
      Z= q.Z-p.Z;
    }

    inline ScalarType Dot(const Vector& V) const { return X*V.X+Y*V.Y+Z*V.Z; }
    inline ScalarType Norm2() const { return X*X+Y*Y+Z*Z; };
    inline ScalarType Norm() const { return std::sqrt(Norm2()); }

    inline Vector Normalized() const { return (*this)/this->Norm(); }
    inline void Normalize() { (*this)= this->Normalized(); }

    inline Vector Cross(const Vector& V) const { return Vector(Y*V.Z-Z*V.Y, Z*V.X-X*V.Z, X*V.Y-Y*V.X); }

    inline ScalarType SinTheta(const Vector& V) const { return (this->Cross(V)).Norm()/(this->Norm()*V.Norm()); }
    inline ScalarType CosTheta(const Vector& V) const { return this->Dot(V)/(this->Norm()*V.Norm()); }
    inline ScalarType CotTheta(const Vector& V) const { return this->CosTheta(V)/this->SinTheta(V); }

    friend Vector operator*(ScalarType s, const Vector& V);
    friend Vector operator*(const Vector& V, ScalarType s);
    friend Vector operator/(const Vector& V, ScalarType d);

    friend std::ostream& operator<<(std::ostream& out, const Vector& V);

    ScalarType X, Y, Z;
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

    /// @brief Get the number of vertex of the mesh. 
    /// @return the number of vertex.
    inline IndexType GetNumberOfVertex() const { return m_Vertices.size(); }
    
    /// @brief Get the number of face of the mesh.
    /// @return the number of face.
    inline IndexType GetNumberOfFace() const { return m_Faces.size(); }

    /// @brief Load and triangulate a mesh from an OFF file. 
    /// @param OFFFile the path of an .off file. 
    void LoadOFF(const std::string& OFFFile);

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
    ScalarType CalculateFaceArea(IndexType iFace) const;

    /// @brief Calculate the area of the patch of surface corresponding to the vertex of index iVertex.
    /// @param iVertex index of the vertex. 
    /// @return a scalar corresponding to the area of the patch.
    ScalarType CalculatePatchAreaForVertex(IndexType iVertex) const;
 
    /// @brief Calculate the Laplacian of a discrete function defined on the mesh.
    /// @param U the scalar function known at each vertex of the mesh. 
    /// @return An array of Laplacian values for each vertex.  
    std::vector<ScalarType> CalculateCotangentLaplacian(std::vector<ScalarType> U) const;

  private: 
    /// @brief Checking the integrity of the mesh structure.
    void IntegrityCheck() const; 
  
  private: 
    std::vector<Vertex> m_Vertices;
    std::vector<Face> m_Faces;
  };

} // namespace GAM