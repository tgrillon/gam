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
  using ScalarType= double;
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
    inline static Vector UnitX() { return {1., 0., 0.}; }
    inline static Vector UnitY() { return {0., 1., 0.}; }
    inline static Vector UnitZ() { return {0., 0., 1.}; }
    inline static Vector Null()  { return {0., 0., 0.}; }

    Vector() : X(0), Y(0), Z(0) {}
    Vector(ScalarType x, ScalarType y, ScalarType z) : X(x), Y(y), Z(z) {}

    Vector(const Vertex& p, const Vertex& q)
    {	
      X= q.X-p.X;
      Y= q.Y-p.Y;
      Z= q.Z-p.Z;
    }

    /// @brief Calculate the dot product between `this` and V.
    /// @param V a vector.
    /// @return the value of the dot product.  
    inline ScalarType Dot(const Vector& V) const { return X*V.X+Y*V.Y+Z*V.Z; }

    /// @brief Calculate the cross product between `this` and V.
    /// @param V a vector.
    /// @return the orthogonal vector to `this` and V.
    inline Vector Cross(const Vector& V) const { return Vector(Y*V.Z-Z*V.Y, Z*V.X-X*V.Z, X*V.Y-Y*V.X); }

    /// @brief Calculate the magnitude²/norm² of `this`.
    /// @return the norm² of `this`.
    inline ScalarType Norm2() const { return X*X+Y*Y+Z*Z; };

    /// @brief Calculate the magnitude/norm of `this`.
    /// @return the norm of `this`.
    inline ScalarType Norm() const { return std::sqrt(Norm2()); }

    /// @brief Normalize `this`.
    inline void Normalize() { (*this)= this->Normalized(); }

    /// @brief Return a normalize vector.
    /// @return the normalized vector of `this`. 
    inline Vector Normalized() const { return (*this)/this->Norm(); }

    /// @brief Calculate the cotangent between `this` and V.
    /// @param V a vector.
    /// @return the value of cot(theta) with theta the angle between `this` and V.
    inline ScalarType Cotan(const Vector& V) const { return this->Dot(V)/(this->Cross(V)).Norm(); }

    friend Vector operator*(ScalarType s, const Vector& V);
    friend Vector operator*(const Vector& V, ScalarType s);
    friend Vector operator/(const Vector& V, ScalarType d);
    friend Vector operator/(const Vector& U, const Vector& V);
    friend Vector operator+(const Vector& U, const Vector& V);
    friend Vector operator-(const Vector& U, const Vector& V);
    friend Vector operator+=(const Vector& U, const Vector& V);
    friend Vector operator-=(const Vector& U, const Vector& V);

    friend std::ostream& operator<<(std::ostream& out, const Vector& V);

    ScalarType X, Y, Z;
  };


  struct Face
  {
    int Vertices[3]  { -1, -1, -1 };
    int Neighbors[3] { -1, -1, -1 };
  };

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
  };

} // namespace GAM