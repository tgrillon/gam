#include "Mesh.hpp"
#include "utils.hpp"

// #define DEBUG

int main(int argc, char** argv)
{
  GAM::Mesh mesh; 
#ifdef DEBUG
  ELAPSED_TIME(mesh.Load(argc > 1 ? argv[1] : "../../data/cube_maillage_triangles.off"));
  ELAPSED_TIME(mesh.PrintNeighboringFacesOfFace(2));
  ELAPSED_TIME(mesh.PrintNeighboringFacesOfVertex(2)); 
#else
  mesh.Load(argc > 1 ? argv[1] : "../../data/cube_maillage_triangles.off");
  mesh.PrintNeighboringFacesOfFace(2);
  mesh.PrintNeighboringFacesOfVertex(2); 
#endif
}