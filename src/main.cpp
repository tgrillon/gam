#include "Mesh.hpp"

#include <chrono>

std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point stop;

std::chrono::microseconds duration;

#define ELAPSED_TIME(func) \
        start = std::chrono::high_resolution_clock::now(); \
        func; \
        stop = std::chrono::high_resolution_clock::now(); \
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); \
        std::cout << "Time taken by " << #func << ": " << (duration.count() / 1000.0) << "ms" << std::endl; \
	

#define DEBUG
// #define CONNECTIVITY_TEST

int main(int argc, char** argv)
{
  GAM::Mesh mesh; 

  std::string fileOFF= argc > 1 ? argv[1] : "../../data/cube_maillage_triangles.off"; 
#ifdef CONNECTIVITY_TEST
#ifdef DEBUG
  ELAPSED_TIME(mesh.LoadOFF(fileOFF));
  ELAPSED_TIME(mesh.PrintNeighboringFacesOfFace(2));
  ELAPSED_TIME(mesh.PrintNeighboringFacesOfVertex(2)); 
#else
  mesh.LoadOFF(fileOFF);
  mesh.PrintNeighboringFacesOfFace(2);
  mesh.PrintNeighboringFacesOfVertex(2); 
#endif
#endif

  mesh.LoadOFF(fileOFF);
  // std::vector<GAM::ScalarType> U(mesh.GetNumberOfVertex());
  // U[0]= 1;
  // mesh.CalculateCotangentLaplacian(U);
  // for (const auto& [i, v] : utils::enumerate(U)) 
    // std::cout << i << ": " << v << "\n";
  mesh.SaveAsOBJ("test.obj");
}