#include "TMesh.h"

#define DEBUG
namespace GAM
{
void GAM::TMesh::load_off(const std::string &OFFFile)
{
  std::ifstream file(std::string(OFF_DIR) + OFFFile);
  // Checking if we can read the file 
  if (!file.is_open())
  {
    utils::error("Couldn't open this file: ", OFFFile);
    exit(1);
  }
  // Checking file type 
  std::string type;
  file >> type; 
  if (type!="OFF")
  {
    utils::error("The format of the provided file must be OFF: ", OFFFile);
    exit(1);
  }
  // Retrieving number of vertices / faces 
  IndexType nVertices, nFaces, nEdges;
  file >> nVertices >> nFaces >> nEdges; 
  m_Vertices.resize(nVertices);
  m_Faces.resize(nFaces);
  m_Values.resize(nVertices, 0.);
  m_Curvature.resize(nVertices, 0.);
  m_Normals.resize(nVertices, Vector::unit_z());

  Point pmin, pmax; 
  // Initializing position of vertices 
  for (IndexType i=0; i<nVertices; ++i)
  { 
    Vertex vertex; 
    file >> vertex.X >> vertex.Y >> vertex.Z;
    m_vertices[i]= vertex;
  }

  std::unordered_map<std::pair<IndexType, IndexType>, std::pair<IndexType, int>, HashEdgePair> map; 
  for (IndexType i=0; i<nFaces; ++i)
  {
    IndexType v0, v1, v2;
    file >> nVertices >> v0 >> v1 >> v2;
    // Set FaceIndex for each vertex if it's not already the case 
    if (m_vertices[v0].FaceIndex==-1) m_vertices[v0].FaceIndex= i; 
    if (m_vertices[v1].FaceIndex==-1) m_vertices[v1].FaceIndex= i; 
    if (m_vertices[v2].FaceIndex==-1) m_vertices[v2].FaceIndex= i; 
    // Set vertex indices of the face i
    m_faces[i].Vertices[0]= v0;
    assert(m_faces[i].Vertices[0]!=-1);
    m_faces[i].Vertices[1]= v1;
    assert(m_faces[i].Vertices[1]!=-1);
    m_faces[i].Vertices[2]= v2;
    assert(m_faces[i].Vertices[2]!=-1);
    // Set neighboring faces using the edges 
    // Edge v0-v1
    if (map.find({v0, v1})==map.end() && map.find({v1, v0})==map.end())
    {
      map[{v0, v1}]= {i, 2};
    }
    else // The edge with v0 and v1 is already registered in map 
    {
      auto [FaceIndex, EdgeIndex]= map[{v1, v0}];
      m_faces[FaceIndex].Neighbors[EdgeIndex]= i;
      m_faces[i].Neighbors[2]= FaceIndex; 
    }
    // Edge v1-v2
    if (map.find({v1, v2})==map.end() && map.find({v2, v1})==map.end())
    {
      map[{v1, v2}]= {i, 0};
    }
    else // The edge with v1 and v2 is already registered in map 
    {
      auto [FaceIndex, EdgeIndex]= map[{v2, v1}];
      m_faces[FaceIndex].Neighbors[EdgeIndex]= i;
      m_faces[i].Neighbors[0]= FaceIndex; 
    }
    // Edge v2-v0
    if (map.find({v2, v0})==map.end() && map.find({v0, v2})==map.end())
    {
      map[{v2, v0}]= {i, 1};
    }
    else // The edge with v2 and v0 is already registered in map 
    {
      auto [FaceIndex, EdgeIndex]= map[{v0, v2}];
      m_faces[FaceIndex].Neighbors[EdgeIndex]= i;
      m_faces[i].Neighbors[1]= FaceIndex; 
    }
  }
#ifdef DEBUG
  integrity_check();
  utils::logln("TMesh successfully loaded!");
#endif
  file.close();
}
void TMesh::save_obj(const std::string &OBJFile, bool useCurvature)
{
  std::ofstream file(std::string(OBJ_DIR) + OBJFile);
  file << "OBJ" << "\n";

  file << vertex_count() << " " << face_count() << " " << 0 << "\n";
  
  // Save vertices position
  for (const auto& v : m_vertices)
  {
    file << "v " << v.X << " " << v.Y << " " << v.Z << "\n";  
  }
  // Save textcoords
  auto& data= useCurvature ? m_Curvature : m_Values;
  for (const auto& v : data)
  {
    file << "vt " << v << " " << v << "\n";  
  }
  // Save vertices normals 
  for (const auto& n : m_normals)
  {
    file << "vn " << n.X << " " << n.Y << " " << n.Z << "\n";  
  }
  // Save topology
  for (const auto& f : m_faces)
  {
    file << "f ";
    file << (f.Vertices[0] + 1) << "/" << (f.Vertices[0] + 1) << "/" << (f.Vertices[0] + 1) << " ";  
    file << (f.Vertices[1] + 1) << "/" << (f.Vertices[1] + 1) << "/" << (f.Vertices[1] + 1) << " ";  
    file << (f.Vertices[2] + 1) << "/" << (f.Vertices[2] + 1) << "/" << (f.Vertices[2] + 1) <<  "\n";
  }
  file.close();
  utils::logln("File ", OBJFile, " successfully saved!");
}

IndexType TMesh::local_index(IndexType iVertex, IndexType iFace) const
{
  assert(iFace < face_count());
  assert(iVertex < vertex_count());
  
  int localIndex= 0; 
  while (m_faces[iFace].Vertices[localIndex]!=iVertex && localIndex<3) localIndex++;
  return localIndex;
}

void TMesh::print_neighboring_faces_of_face(IndexType iFace) const
{
  assert(iFace < face_count());

  int i= 0; 
  utils::logln("--Neighboring faces of face ", iFace, "--");
  for (auto face : m_faces[iFace].Neighbors)
  {
    utils::logln("Neighboring face ", i++, ": ", face);
  }
  std::cout << std::endl;
}

void TMesh::print_neighboring_faces_of_vertex(IndexType iVertex) const
{
  assert(iVertex < face_count());

  auto& faceStartIndex= m_vertices[iVertex].FaceIndex; 
  utils::logln("--Neighboring faces of vertex ", iVertex, "--");
  int i= 0; 
  utils::logln("Neighboring face ", i++, ": ", faceStartIndex);
  IndexType localVertexIndex= local_index(iVertex, faceStartIndex);
  IndexType currentFaceIndex= m_faces[faceStartIndex].Neighbors[(localVertexIndex+1)%3];
  while (currentFaceIndex!=faceStartIndex)
  {
    utils::logln("Neighboring face ", i++, ": ", currentFaceIndex);
    localVertexIndex= local_index(iVertex, currentFaceIndex);
    currentFaceIndex= m_faces[currentFaceIndex].Neighbors[(localVertexIndex+1)%3];
  } 
  std::cout << std::endl;
}

std::vector<IndexType> TMesh::neighboring_faces_of_face(IndexType iFace) const
{
  assert(iFace < face_count());

  std::vector<IndexType> neighbors;  
  for (auto face : m_faces[iFace].Neighbors)
  {
    neighbors.emplace_back(face);
  }
  return neighbors;
}

std::vector<IndexType> TMesh::neighboring_faces_of_vertex(IndexType iVertex) const
{
  assert(iVertex < vertex_count());

  std::vector<IndexType> neighbors;
  neighbors.emplace_back(m_vertices[iVertex].FaceIndex); 
  IndexType localVertexIndex= local_index(iVertex, neighbors[0]);
  IndexType currentFaceIndex= m_faces[neighbors[0]].Neighbors[(localVertexIndex+1)%3];
  while (currentFaceIndex!=neighbors[0])
  {
    neighbors.emplace_back(currentFaceIndex); 
    localVertexIndex= local_index(iVertex, currentFaceIndex);
    currentFaceIndex= m_faces[currentFaceIndex].Neighbors[(localVertexIndex+1)%3];
  } 
  return neighbors;
}

std::vector<IndexType> TMesh::neighboring_vertices_of_vertex(IndexType iVertex) const
{
  assert(iVertex  < vertex_count());

  assert(m_vertices[iVertex].FaceIndex!=-1);
  std::vector<IndexType> neighbors;
  IndexType startFaceIndex= m_vertices[iVertex].FaceIndex;  
  IndexType currentFaceIndex= startFaceIndex;  
  IndexType localIndex= local_index(iVertex, currentFaceIndex);
  IndexType nextVertexLocalIndex= (localIndex+1)%3;
  IndexType nextVertexGlobalIndex= m_faces[currentFaceIndex].Vertices[nextVertexLocalIndex];
  neighbors.emplace_back(nextVertexGlobalIndex);
  currentFaceIndex= m_faces[currentFaceIndex].Neighbors[nextVertexLocalIndex];
  while (currentFaceIndex!=startFaceIndex)
  {
    localIndex= local_index(iVertex, currentFaceIndex);
    nextVertexLocalIndex= (localIndex+1)%3;
    nextVertexGlobalIndex= m_faces[currentFaceIndex].Vertices[nextVertexLocalIndex];
    neighbors.emplace_back(nextVertexGlobalIndex);
    currentFaceIndex= m_faces[currentFaceIndex].Neighbors[nextVertexLocalIndex];
  }
  return neighbors;
}

ScalarType TMesh::face_area(IndexType iFace) const
{
  assert(iFace < face_count());

  const auto& f= m_faces[iFace];
  const Vertex& v0= m_vertices[f.Vertices[0]];
  const Vertex& v1= m_vertices[f.Vertices[1]];
  const Vertex& v2= m_vertices[f.Vertices[2]];
  Vector v0v1(v0, v1);
  Vector v0v2(v0, v2);
  Vector n= v0v1.cross(v0v2);
  
  return 0.5*n.norm();
}

ScalarType TMesh::patch_area(IndexType iVertex) const
{
  assert(iVertex < vertex_count());
  auto iFaces= neighboring_faces_of_vertex(iVertex);
  ScalarType area= 0.f;  
  for (auto iFace : iFaces)
  {
    area+= face_area(iFace);
  }
  return area/3;
}

void TMesh::laplacian() 
{
  assert(m_values.size() == m_vertices.size());
  for (IndexType i= 1; i < m_vertices.size(); ++i)
  { 
    m_values[i]= laplacian(i);
  }
}

Vector TMesh::face_normal(IndexType iFace) const
{
  assert(iFace < face_count());
  
  auto faceVertices= m_faces[iFace].Vertices;

  Vector u(m_vertices[faceVertices[0]], m_vertices[faceVertices[1]]);
  Vector v(m_vertices[faceVertices[0]], m_vertices[faceVertices[2]]);

  Vector faceNormal= u.cross(v);

  return faceNormal.normalized();
}

void TMesh::smooth_normals()
{
  assert(m_normals.size() == m_vertices.size());

  for (IndexType i= 0; i < m_vertices.size(); ++i)
  { 
    Vector laplacian= laplacian_vector(i);
    
    IndexType iFace= m_vertices[i].FaceIndex;
    Vector faceNormal= face_normal(iFace);

    Vector vertexNormal= laplacian; 
    if (faceNormal.dot(laplacian) < 0) 
    {
      vertexNormal= -vertexNormal; 
    }
   
    m_Curvature[i]= vertexNormal.norm();
    m_Normals[i]= vertexNormal; 
  }
}

void TMesh::curvature()
{
  ScalarType maxCurv= *std::max_element(m_Curvature.begin(), m_Curvature.end()); 

  assert(maxCurv > 0 || maxCurv < 0);

  for (IndexType i= 0; i < m_Vertices.size(); ++i) 
  {
    m_Curvature[i]= m_Curvature[i] / maxCurv;
  }
}

void TMesh::heat_diffusion(ScalarType deltaTime)
{
  for (int i= 1; i < number_of_vertices(); ++i)
  {
    heat_diffusion(i, deltaTime);
  }
}

void TMesh::heat_diffusion(IndexType iVertex, ScalarType deltaTime)
{
  assert(iVertex < vertex_count());

  m_values[iVertex]+= deltaTime * laplacian(iVertex); 
}

ScalarType TMesh::laplacian(IndexType iVertex)
{
  assert(m_values.size() == m_vertices.size());

  ScalarType Lui= 0.;
  auto neighboringVertices= neighboring_vertices_of_vertex(iVertex);  
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
    ScalarType cotAlpha= v.cotan(w); 
    v= Vector(m_Vertices[nextJVertex], m_Vertices[iVertex]);
    w= Vector(m_Vertices[nextJVertex], m_Vertices[jVertex]);
    ScalarType cotBeta= v.cotan(w); 
    ScalarType& ui= m_Values[iVertex];
    ScalarType& uj= m_Values[jVertex]; 
    
    Lui+= (cotAlpha+cotBeta)*(uj-ui);
  }
  return Lui/(2.*patch_area(iVertex));
}

Vector TMesh::laplacian_vector(IndexType iVertex) 
{
  assert(m_values.size() == m_vertices.size());
  Vector laplacian;
  auto neighboringVertices= neighboring_vertices_of_vertex(iVertex);  
  auto numOfNeighbors= neighboringVertices.size();
  for (size_t j= 1; j <= numOfNeighbors; ++j)
  {
    assert(j-1 >= 0);
    IndexType jVertex= neighboringVertices[j%numOfNeighbors];
    IndexType prevJVertex= neighboringVertices[j-1]; 
    IndexType nextJVertex= neighboringVertices[(j+1)%numOfNeighbors]; 
    
    // Cotangente 
    Vector pj(m_vertices[prevJVertex], m_vertices[jVertex]); 
    Vector pi(m_vertices[prevJVertex], m_vertices[iVertex]);
    ScalarType cotAlpha= pj.cotan(pi); 
    Vector ni= Vector(m_vertices[nextJVertex], m_vertices[iVertex]);
    Vector nj= Vector(m_vertices[nextJVertex], m_vertices[jVertex]);
    ScalarType cotBeta= ni.cotan(nj); 
    auto& vi= m_vertices[iVertex];
    auto& vj= m_vertices[jVertex];
    Vector ui= Vector(vi.X, vi.Y, vi.Z);
    Vector uj= Vector(vj.X, vj.Y, vj.Z);
    
    laplacian.X+= (cotAlpha+cotBeta)*(uj.X-ui.X);
    laplacian.Y+= (cotAlpha+cotBeta)*(uj.Y-ui.Y);
    laplacian.Z+= (cotAlpha+cotBeta)*(uj.Z-ui.Z);
  }
  laplacian= laplacian/(2.*patch_area(iVertex));
  
  return laplacian;
}

void TMesh::integrity_check() const
{
  for (IndexType i= 0; i<m_vertices.size(); ++i)
  {
    auto face= m_faces[m_vertices[i].FaceIndex];
    assert(face.Vertices[0]==i ||
            face.Vertices[1]==i ||
            face.Vertices[2]==i);  
  }
}
} // namespace GAM