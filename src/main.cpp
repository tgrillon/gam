#include "Mesh.hpp"

int main(int argc, char** argv)
{
  GAM::Mesh mesh; 
  mesh.Load(argc > 1 ? argv[1] : "../../data/cube_maillage_triangles.off");
  // mesh.PrintNeighboringFacesOfFace(2);
  // mesh.PrintNeighboringFacesOfVertex(2); 
  for (auto iFace : mesh.GetNeighboringFacesOfVertex(2))
  {
    std::cout << "Face : " << iFace << "\n";
  }
}