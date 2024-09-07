#include "Mesh.hpp"

namespace GAM
{
  void GAM::Mesh::Load(const std::string &OFFFile)
  {
    std::ifstream file(OFFFile, std::ios::in);

    // Checking if we can read the file 
    if (!file.is_open())
    {
      std::cerr << "Couldn't open this file: " << OFFFile << "\n";
      return;
    }

    // Checking file type 
    std::string type;
    file >> type; 

    assert(type == "OFF");

    // Retrieving number of vertices / faces 
    size_t nVertices, nFaces, nEdges;
    file >> nVertices >> nFaces >> nEdges; 

    m_Vertices.resize(nVertices);
    m_Faces.resize(nFaces);

    // Initializing position of vertices 
    for (int i = 0; i < nVertices; ++i)
    { 
      Vertex vertex; 
      file >> vertex.X >> vertex.Y >> vertex.Z;

      m_Vertices[i] = vertex;
    }

    std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashEdgePair> map; 

    for (int i = 0; i < nFaces; ++i)
    {
      size_t v0, v1, v2;
      file >> nVertices >> v0 >> v1 >> v2;

      // Set FaceIndex for each vertex if it's not already the case 
      if (m_Vertices[v0].FaceIndex == -1) m_Vertices[v0].FaceIndex = i; 
      if (m_Vertices[v1].FaceIndex == -1) m_Vertices[v1].FaceIndex = i; 
      if (m_Vertices[v2].FaceIndex == -1) m_Vertices[v2].FaceIndex = i; 

      // Set vertex indices of the face i
      m_Faces[i].Vertices[0] = v0;
      assert(m_Faces[i].Vertices[0] != -1);
      m_Faces[i].Vertices[1] = v1;
      assert(m_Faces[i].Vertices[1] != -1);
      m_Faces[i].Vertices[2] = v2;
      assert(m_Faces[i].Vertices[2] != -1);

      if (map.find({v0, v1}) == map.end() && map.find({v1, v0}) == map.end())
      {
        map[{v0, v1}] = {i, 2};
      }
      else 
      {
        auto [FaceIndex, EdgeIndex] = map[{v1, v0}];
        m_Faces[FaceIndex].Neighbors[EdgeIndex] = i;
        m_Faces[i].Neighbors[2] = FaceIndex; 
      }

      if (map.find({v1, v2}) == map.end() && map.find({v2, v1}) == map.end())
      {
        map[{v1, v2}] = {i, 0};
      }
      else 
      {
        auto [FaceIndex, EdgeIndex] = map[{v2, v1}];
        m_Faces[FaceIndex].Neighbors[EdgeIndex] = i;
        m_Faces[i].Neighbors[0] = FaceIndex; 
      }

      if (map.find({v2, v0}) == map.end() && map.find({v0, v2}) == map.end())
      {
        map[{v2, v0}] = {i, 1};
      }
      else 
      {
        auto [FaceIndex, EdgeIndex] = map[{v0, v2}];
        m_Faces[FaceIndex].Neighbors[EdgeIndex] = i;
        m_Faces[i].Neighbors[1] = FaceIndex; 
      }
    }

    file.close();
  }

  size_t Mesh::GetVertexLocalIndex(size_t iVertex, size_t iFace) const
  {
    int localIndex = 0; 

    while (m_Faces[iFace].Vertices[localIndex] != iVertex && localIndex < 3) localIndex++;

    return localIndex;
  }
  
  void Mesh::PrintNeighboringFacesOfFace(size_t iFace) const
  {
    int i = 0; 
    std::cout << "--Neighboring faces of face " << iFace << "--\n";
    for (auto face : m_Faces[iFace].Neighbors)
    {
      std::cout << "Neighboring face " << i++ << ": " << face << "\n";
    }
    std::cout << "\n";
  }

  void Mesh::PrintNeighboringFacesOfVertex(size_t iVertex) const
  {
    auto& faceStartIndex = m_Vertices[iVertex].FaceIndex; 
    std::cout << "--Neighboring faces of vertex " << iVertex << "--\n";
    int i = 0; 
    std::cout << "Neighboring face " << i++ << ": " << faceStartIndex << "\n";

    size_t localVertexIndex = GetVertexLocalIndex(iVertex, faceStartIndex);
    size_t currentFaceIndex = m_Faces[faceStartIndex].Neighbors[(localVertexIndex+1)%3];
    while (currentFaceIndex != faceStartIndex)
    {
      std::cout << "Neighboring face " << i++ << ": " << currentFaceIndex << "\n";
      localVertexIndex = GetVertexLocalIndex(iVertex, currentFaceIndex);
      currentFaceIndex = m_Faces[currentFaceIndex].Neighbors[(localVertexIndex+1)%3];
    } 
    std::cout << "\n";
  }

  std::vector<size_t> Mesh::GetNeighboringFacesOfFace(size_t iFace) const
  {
    std::vector<size_t> neighbors;  
    for (auto face : m_Faces[iFace].Neighbors)
    {
      neighbors.emplace_back(face);
    }
    return neighbors;
  }
  
  std::vector<size_t> Mesh::GetNeighboringFacesOfVertex(size_t iVertex) const
  {
    std::vector<size_t> neighbors;

    neighbors.emplace_back(m_Vertices[iVertex].FaceIndex); 
    size_t localVertexIndex = GetVertexLocalIndex(iVertex, neighbors[0]);
    size_t currentFaceIndex = m_Faces[neighbors[0]].Neighbors[(localVertexIndex+1)%3];
    while (currentFaceIndex != neighbors[0])
    {
      neighbors.emplace_back(currentFaceIndex); 
      localVertexIndex = GetVertexLocalIndex(iVertex, currentFaceIndex);
      currentFaceIndex = m_Faces[currentFaceIndex].Neighbors[(localVertexIndex+1)%3];
    } 

    return neighbors;
  }
} // namespace GAM
