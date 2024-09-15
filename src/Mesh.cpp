#include "Mesh.hpp"

#define DEBUG

namespace GAM
{
  void GAM::Mesh::LoadOFF(const std::string &OFFFile)
  {
    std::ifstream file(OFFFile);

    // Checking if we can read the file 
    if (!file.is_open())
    {
      std::cerr << "Couldn't open this file: " << OFFFile << "\n";
      exit(1);
    }

    // Checking file type 
    std::string type;
    file >> type; 

    if (type!="OFF")
    {
      std::cerr << "The format of the provided file must be OFF: " << OFFFile << "\n";
      exit(1);
    }

    // Retrieving number of vertices / faces 
    IndexType nVertices, nFaces, nEdges;
    file >> nVertices >> nFaces >> nEdges; 

    m_Vertices.resize(nVertices);
    m_Faces.resize(nFaces);

    // Initializing position of vertices 
    for (IndexType i=0; i<nVertices; ++i)
    { 
      Vertex vertex; 
      file >> vertex.X >> vertex.Y >> vertex.Z;

      m_Vertices[i]= vertex;
    }

    std::unordered_map<std::pair<IndexType, IndexType>, std::pair<IndexType, int>, HashEdgePair> map; 

    for (IndexType i=0; i<nFaces; ++i)
    {
      IndexType v0, v1, v2;
      file >> nVertices >> v0 >> v1 >> v2;

      // Set FaceIndex for each vertex if it's not already the case 
      if (m_Vertices[v0].FaceIndex==-1) m_Vertices[v0].FaceIndex= i; 
      if (m_Vertices[v1].FaceIndex==-1) m_Vertices[v1].FaceIndex= i; 
      if (m_Vertices[v2].FaceIndex==-1) m_Vertices[v2].FaceIndex= i; 

      // Set vertex indices of the face i
      m_Faces[i].Vertices[0]= v0;
      assert(m_Faces[i].Vertices[0]!=-1);
      m_Faces[i].Vertices[1]= v1;
      assert(m_Faces[i].Vertices[1]!=-1);
      m_Faces[i].Vertices[2]= v2;
      assert(m_Faces[i].Vertices[2]!=-1);

      // Set neighboring faces using the edges 
      // Edge v0-v1
      if (map.find({v0, v1})==map.end() && map.find({v1, v0})==map.end())
      {
        map[{v0, v1}]= {i, 2};
      }
      else // The edge with v0 and v1 is already registered in map 
      {
        auto [FaceIndex, EdgeIndex]= map[{v1, v0}];
        m_Faces[FaceIndex].Neighbors[EdgeIndex]= i;
        m_Faces[i].Neighbors[2]= FaceIndex; 
      }

      // Edge v1-v2
      if (map.find({v1, v2})==map.end() && map.find({v2, v1})==map.end())
      {
        map[{v1, v2}]= {i, 0};
      }
      else // The edge with v1 and v2 is already registered in map 
      {
        auto [FaceIndex, EdgeIndex]= map[{v2, v1}];
        m_Faces[FaceIndex].Neighbors[EdgeIndex]= i;
        m_Faces[i].Neighbors[0]= FaceIndex; 
      }

      // Edge v2-v0
      if (map.find({v2, v0})==map.end() && map.find({v0, v2})==map.end())
      {
        map[{v2, v0}]= {i, 1};
      }
      else // The edge with v2 and v0 is already registered in map 
      {
        auto [FaceIndex, EdgeIndex]= map[{v0, v2}];
        m_Faces[FaceIndex].Neighbors[EdgeIndex]= i;
        m_Faces[i].Neighbors[1]= FaceIndex; 
      }
    }

#ifdef DEBUG
    IntegrityCheck();
    std::cout << "Mesh successfully loaded!\n";
#endif

    file.close();
  }

  IndexType Mesh::GetVertexLocalIndex(IndexType iVertex, IndexType iFace) const
  {
    assert(iFace<m_Faces.size());
    assert(iVertex<m_Vertices.size());

    int localIndex= 0; 

    while (m_Faces[iFace].Vertices[localIndex]!=iVertex && localIndex<3) localIndex++;

    return localIndex;
  }
  
  void Mesh::PrintNeighboringFacesOfFace(IndexType iFace) const
  {
    assert(iFace<m_Faces.size());

    int i= 0; 
    std::cout << "--Neighboring faces of face " << iFace << "--\n";
    for (auto face : m_Faces[iFace].Neighbors)
    {
      std::cout << "Neighboring face " << i++ << ": " << face << "\n";
    }
    std::cout << std::endl;
  }

  void Mesh::PrintNeighboringFacesOfVertex(IndexType iVertex) const
  {
    assert(iVertex<m_Vertices.size());

    auto& faceStartIndex= m_Vertices[iVertex].FaceIndex; 
    std::cout << "--Neighboring faces of vertex " << iVertex << "--\n";
    int i= 0; 
    std::cout << "Neighboring face " << i++ << ": " << faceStartIndex << "\n";

    IndexType localVertexIndex= GetVertexLocalIndex(iVertex, faceStartIndex);
    IndexType currentFaceIndex= m_Faces[faceStartIndex].Neighbors[(localVertexIndex+1)%3];
    while (currentFaceIndex!=faceStartIndex)
    {
      std::cout << "Neighboring face " << i++ << ": " << currentFaceIndex << "\n";
      localVertexIndex= GetVertexLocalIndex(iVertex, currentFaceIndex);
      currentFaceIndex= m_Faces[currentFaceIndex].Neighbors[(localVertexIndex+1)%3];
    } 
    std::cout << std::endl;
  }

  std::vector<IndexType> Mesh::GetNeighboringFacesOfFace(IndexType iFace) const
  {
    assert(iFace<m_Faces.size());

    std::vector<IndexType> neighbors;  
    for (auto face : m_Faces[iFace].Neighbors)
    {
      neighbors.emplace_back(face);
    }
    return neighbors;
  }
  
  std::vector<IndexType> Mesh::GetNeighboringFacesOfVertex(IndexType iVertex) const
  {
    assert(iVertex<m_Vertices.size());

    std::vector<IndexType> neighbors;

    neighbors.emplace_back(m_Vertices[iVertex].FaceIndex); 
    IndexType localVertexIndex= GetVertexLocalIndex(iVertex, neighbors[0]);
    IndexType currentFaceIndex= m_Faces[neighbors[0]].Neighbors[(localVertexIndex+1)%3];
    while (currentFaceIndex!=neighbors[0])
    {
      neighbors.emplace_back(currentFaceIndex); 
      localVertexIndex= GetVertexLocalIndex(iVertex, currentFaceIndex);
      currentFaceIndex= m_Faces[currentFaceIndex].Neighbors[(localVertexIndex+1)%3];
    } 

    return neighbors;
  }

  std::vector<IndexType> Mesh::GetNeighboringVerticesOfVertex(IndexType iVertex) const
  {
    assert(iVertex<m_Vertices.size());
    assert(m_Vertices[iVertex].FaceIndex!=-1);

    std::vector<IndexType> neighbors;
    IndexType startFaceIndex= m_Vertices[iVertex].FaceIndex;  
    IndexType currentFaceIndex= startFaceIndex;  
    IndexType localIndex= GetVertexLocalIndex(iVertex, currentFaceIndex);
    IndexType nextVertexLocalIndex= (localIndex+1)%3;
    IndexType nextVertexGlobalIndex= m_Faces[currentFaceIndex].Vertices[nextVertexLocalIndex];
    neighbors.emplace_back(nextVertexGlobalIndex);
    currentFaceIndex= m_Faces[currentFaceIndex].Neighbors[nextVertexLocalIndex];
    while (currentFaceIndex!=startFaceIndex)
    {
      localIndex= GetVertexLocalIndex(iVertex, currentFaceIndex);
      nextVertexLocalIndex= (localIndex+1)%3;
      nextVertexGlobalIndex= m_Faces[currentFaceIndex].Vertices[nextVertexLocalIndex];
      neighbors.emplace_back(nextVertexGlobalIndex);
      currentFaceIndex= m_Faces[currentFaceIndex].Neighbors[nextVertexLocalIndex];
    }

    return neighbors;
  }

  ScalarType Mesh::CalculateFaceArea(IndexType iFace) const
  {
    assert(iFace<m_Faces.size());

    const auto& f= m_Faces[iFace];
    const Vertex& v0= m_Vertices[f.Vertices[0]];
    const Vertex& v1= m_Vertices[f.Vertices[1]];
    const Vertex& v2= m_Vertices[f.Vertices[2]];
    Vector v0v1(v0, v1);
    Vector v0v2(v0, v2);

    Vector n= v0v1.Cross(v0v2);
    
    return n.Norm()*0.5;
  }

  ScalarType Mesh::CalculatePatchAreaForVertex(IndexType iVertex) const
  {
    assert(iVertex<m_Vertices.size());

    auto iFaces= GetNeighboringFacesOfVertex(iVertex);

    ScalarType area= 0.f;  
    for (auto iFace : iFaces)
    {
      area+= CalculateFaceArea(iFace);
    }

    return area/3;
  }

  std::vector<ScalarType> Mesh::CalculateCotangentLaplacian(std::vector<ScalarType> U) const
  {
    assert(U.size()==m_Vertices.size());

    std::vector<ScalarType> laplacians(U.size());
    for (const auto& [index, vertex] : utils::enumerate(m_Vertices))
    { 
      ScalarType Lui= 0.;
      auto neighboringVertices= GetNeighboringVerticesOfVertex(index);  
      auto numOfNeighbors= neighboringVertices.size();
      for (size_t j= 1; j <= numOfNeighbors; ++j)
      {
        assert(j-1<numOfNeighbors && j-1>=0);
        IndexType& vertexJIndex= neighboringVertices[j%numOfNeighbors];
        // Neighbor Vertex Index Minus 1
        IndexType& vertexJPrevIndex= neighboringVertices[j-1]; 
        // Neighbor Vertex Index Plus 1
        IndexType& vertexJNextIndex= neighboringVertices[(j+1)%numOfNeighbors]; 
        ScalarType uj= U[vertexJIndex]; 
        ScalarType ui= U[index];
        Vector v(m_Vertices[vertexJPrevIndex], m_Vertices[vertexJIndex]); 
        Vector w(m_Vertices[vertexJPrevIndex], vertex);
        ScalarType cotAlpha= v.Cotan(w); 
        v= Vector(m_Vertices[vertexJNextIndex], vertex);
        w= Vector(m_Vertices[vertexJNextIndex], m_Vertices[vertexJIndex]);
        ScalarType cotBeta= v.Cotan(w); 
        Lui+= (cotAlpha+cotBeta)*(uj-ui);
      }
      laplacians[index]= Lui/(2.*CalculatePatchAreaForVertex(index));
    }

    return laplacians; 
  }

  void Mesh::IntegrityCheck() const
  {
    for (IndexType i= 0; i<m_Vertices.size(); ++i)
    {
      auto face= m_Faces[m_Vertices[i].FaceIndex];

      assert(face.Vertices[0]==i ||
             face.Vertices[1]==i ||
             face.Vertices[2]==i);  
    }
  }
  
  Vector operator*(ScalarType s, const Vector &V)
  {
    return Vector(s*V.X, s*V.Y, s*V.Z);
  }

  Vector operator*(const Vector &V, ScalarType s)
  {
    return s*V;
  }

  Vector operator/(const Vector &V, ScalarType d)
  {
    assert(d > 0. || d<0.);
    ScalarType id= 1./d;
    return id*V;
  }

  std::ostream &operator<<(std::ostream &out, const Vector &V)
  {
    out << "Vector : [" << V.X << ", " << V.Y << ", " << V.Z << "]";
    return out;
  }

  std::ostream &operator<<(std::ostream &out, const Vertex &V)
  {
    out << "Vertex : [" << V.X << ", " << V.Y << ", " << V.Z << "]";
    return out;
  }

} // namespace GAM
