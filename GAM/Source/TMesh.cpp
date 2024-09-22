#include "TMesh.h"

#define DEBUG
namespace GAM
{
void GAM::TMesh::LoadOFF(const std::string &OFFFile)
{
  std::ifstream file(std::string(OFF_DIR) + OFFFile);
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
  m_Values.resize(nVertices, 0.);
  m_Normals.resize(nVertices, Vector::UnitZ());

  Point pmin, pmax; 
  // Initializing position of vertices 
  for (IndexType i=0; i<nVertices; ++i)
  { 
    Vertex vertex; 
    file >> vertex.X >> vertex.Y >> vertex.Z;
    m_Vertices[i]= vertex;

    m_Bbox.Min()= Point::Min(m_Bbox.GetMin(), Vertex::AsPoint(vertex));
    m_Bbox.Max()= Point::Max(m_Bbox.GetMax(), Vertex::AsPoint(vertex));
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
  std::cout << "TMesh successfully loaded!\n";
#endif
  file.close();
}
void TMesh::SaveOBJ(const std::string &OBJFile)
{
  std::ofstream file(std::string(OBJ_DIR) + OBJFile);
  file << "OBJ" << "\n";

  file << GetNumberOfVertex() << " " << GetNumberOfFace() << " " << 0 << "\n";
  
  // Save vertices position
  for (const auto& v : m_Vertices)
  {
    file << "v " << v.X << " " << v.Y << " " << v.Z << "\n";  
  }
  // Save textcoords
  for (const auto& v : m_Values)
  {
    file << "vt " << v << " " << v << "\n";  
  }
  // Save vertices normals 
  for (const auto& n : m_Normals)
  {
    file << "vn " << n.X << " " << n.Y << " " << n.Z << "\n";  
  }
  // Save topology
  for (const auto& f : m_Faces)
  {
    file << "f ";
    file << (f.Vertices[0] + 1) << "/" << (f.Vertices[0] + 1) << "/" << (f.Vertices[0] + 1) << " ";  
    file << (f.Vertices[1] + 1) << "/" << (f.Vertices[0] + 1) << "/" << (f.Vertices[1] + 1) << " ";  
    file << (f.Vertices[2] + 1) << "/" << (f.Vertices[0] + 1) << "/" << (f.Vertices[2] + 1) <<  "\n";
  }
  file.close();
  std::cout << "File " << OBJFile << " successfully saved!" << std::endl;
}

IndexType TMesh::GetVertexLocalIndex(IndexType iVertex, IndexType iFace) const
{
  assert(iFace<m_Faces.size());
  assert(iVertex<m_Vertices.size());
  int localIndex= 0; 
  while (m_Faces[iFace].Vertices[localIndex]!=iVertex && localIndex<3) localIndex++;
  return localIndex;
}

void TMesh::PrintNeighboringFacesOfFace(IndexType iFace) const
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

void TMesh::PrintNeighboringFacesOfVertex(IndexType iVertex) const
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

std::vector<IndexType> TMesh::GetNeighboringFacesOfFace(IndexType iFace) const
{
  assert(iFace<m_Faces.size());
  std::vector<IndexType> neighbors;  
  for (auto face : m_Faces[iFace].Neighbors)
  {
    neighbors.emplace_back(face);
  }
  return neighbors;
}

std::vector<IndexType> TMesh::GetNeighboringFacesOfVertex(IndexType iVertex) const
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

std::vector<IndexType> TMesh::GetNeighboringVerticesOfVertex(IndexType iVertex) const
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

ScalarType TMesh::FaceArea(IndexType iFace) const
{
  assert(iFace<m_Faces.size());
  const auto& f= m_Faces[iFace];
  const Vertex& v0= m_Vertices[f.Vertices[0]];
  const Vertex& v1= m_Vertices[f.Vertices[1]];
  const Vertex& v2= m_Vertices[f.Vertices[2]];
  Vector v0v1(v0, v1);
  Vector v0v2(v0, v2);
  Vector n= v0v1.Cross(v0v2);
  
  return 0.5*n.Norm();
}

ScalarType TMesh::PatchAreaForVertex(IndexType iVertex) const
{
  assert(iVertex<m_Vertices.size());
  auto iFaces= GetNeighboringFacesOfVertex(iVertex);
  ScalarType area= 0.f;  
  for (auto iFace : iFaces)
  {
    area+= FaceArea(iFace);
  }
  return area/3;
}

void TMesh::CotangentLaplacian() 
{
  assert(m_Values.size() == m_Vertices.size());
  for (IndexType i= 1; i < m_Vertices.size(); ++i)
  { 
    m_Values[i]= CotangentLaplacian(i);
  }
}

void TMesh::ComputeNormals()
{
  ScalarType maxCurv= std::numeric_limits<ScalarType>::min();
  ScalarType minCurv= std::numeric_limits<ScalarType>::max();

  assert(m_Normals.size() == m_Vertices.size());
  for (IndexType i= 1; i < m_Vertices.size(); ++i)
  { 
    Vector vertexNormal= VertexNormal(i);
    
    IndexType iFace= m_Vertices[i].FaceIndex;
    auto faceVertices= m_Faces[iFace].Vertices;

    Vector u(m_Vertices[faceVertices[0]], m_Vertices[faceVertices[1]]);
    Vector v(m_Vertices[faceVertices[0]], m_Vertices[faceVertices[2]]);

    Vector faceNormal= u.Cross(v);

    m_Values[i]= vertexNormal.Norm();
    maxCurv= std::max(maxCurv, m_Values[i]);
    minCurv= std::min(minCurv, m_Values[i]);
    if (faceNormal.Dot(vertexNormal) < 0) 
    {
      vertexNormal= -vertexNormal; 
    }

    m_Normals[i]= vertexNormal; 
  }

  for (IndexType i= 1; i < m_Vertices.size(); ++i) 
  {
    m_Values[i]= (m_Values[i] - minCurv) / (maxCurv - minCurv);
  }
}

ScalarType TMesh::CotangentLaplacian(IndexType iVertex)
{
  assert(m_Values.size() == m_Vertices.size());
  ScalarType Lui= 0.;
  auto neighboringVertices= GetNeighboringVerticesOfVertex(iVertex);  
  auto numOfNeighbors= neighboringVertices.size();
  for (size_t j= 1; j <= numOfNeighbors; ++j)
  {
    assert(j-1 >= 0);
    IndexType jVertex= neighboringVertices[j%numOfNeighbors];
    IndexType prevJVertex= neighboringVertices[j-1]; 
    IndexType nextJVertex= neighboringVertices[(j+1)%numOfNeighbors]; 
    
    // Cotangente 
    Vector v(m_Vertices[prevJVertex], m_Vertices[jVertex]); 
    Vector w(m_Vertices[prevJVertex], m_Vertices[iVertex]);
    ScalarType cotAlpha= v.Cotan(w); 
    v= Vector(m_Vertices[nextJVertex], m_Vertices[iVertex]);
    w= Vector(m_Vertices[nextJVertex], m_Vertices[jVertex]);
    ScalarType cotBeta= v.Cotan(w); 
    ScalarType& ui= m_Values[iVertex];
    ScalarType& uj= m_Values[jVertex]; 
    
    Lui+= (cotAlpha+cotBeta)*(uj-ui);
  }
  return Lui/(2.*PatchAreaForVertex(iVertex));
}

Vector TMesh::VertexNormal(IndexType iVertex) 
{
  assert(m_Values.size() == m_Vertices.size());
  Vector laplacian;
  auto neighboringVertices= GetNeighboringVerticesOfVertex(iVertex);  
  auto numOfNeighbors= neighboringVertices.size();
  for (size_t j= 1; j <= numOfNeighbors; ++j)
  {
    assert(j-1 >= 0);
    IndexType jVertex= neighboringVertices[j%numOfNeighbors];
    IndexType prevJVertex= neighboringVertices[j-1]; 
    IndexType nextJVertex= neighboringVertices[(j+1)%numOfNeighbors]; 
    
    // Cotangente 
    Vector pj(m_Vertices[prevJVertex], m_Vertices[jVertex]); 
    Vector pi(m_Vertices[prevJVertex], m_Vertices[iVertex]);
    ScalarType cotAlpha= pj.Cotan(pi); 
    Vector ni= Vector(m_Vertices[nextJVertex], m_Vertices[iVertex]);
    Vector nj= Vector(m_Vertices[nextJVertex], m_Vertices[jVertex]);
    ScalarType cotBeta= ni.Cotan(nj); 
    auto& vi= m_Vertices[iVertex];
    auto& vj= m_Vertices[jVertex];
    Vector ui= Vector(vi.X, vi.Y, vi.Z);
    Vector uj= Vector(vj.X, vj.Y, vj.Z);
    
    laplacian.X+= (cotAlpha+cotBeta)*(uj.X-ui.X);
    laplacian.Y+= (cotAlpha+cotBeta)*(uj.Y-ui.Y);
    laplacian.Z+= (cotAlpha+cotBeta)*(uj.Z-ui.Z);
  }
  laplacian= laplacian/(2.*PatchAreaForVertex(iVertex));
  
  return laplacian;
}

void TMesh::IntegrityCheck() const
{
  for (IndexType i= 0; i<m_Vertices.size(); ++i)
  {
    auto face= m_Faces[m_Vertices[i].FaceIndex];
    assert(face.Vertices[0]==i ||
            face.Vertices[1]==i ||
            face.Vertices[2]==i);  
  }
}
} // namespace GAM