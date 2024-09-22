#include "TMesh.h"
#include "Application.h"

#include <chrono>

std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point stop;

std::chrono::microseconds duration;

#define MEASURE_TIME(func) \
        start = std::chrono::high_resolution_clock::now(); \
        func; \
        stop = std::chrono::high_resolution_clock::now(); \
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); \
        std::cout << "Time taken by " << #func << ": " << (duration.count() / 1000.0) << "ms" << std::endl; \
	

#define DEBUG
// #define CONNECTIVITY_TEST

int main(int argc, char** argv)
{
  GAM::TMesh mesh; 
  std::string filename= argc > 1 ? argv[1] : "cube_maillage_triangles"; 
  std::string fileOFF= "/" + filename + ".off"; 
#ifdef CONNECTIVITY_TEST
#ifdef DEBUG
  MEASURE_TIME(mesh.LoadOFF(fileOFF));
  MEASURE_TIME(mesh.PrintNeighboringFacesOfFace(2));
  MEASURE_TIME(mesh.PrintNeighboringFacesOfVertex(2)); 
#else
  mesh.LoadOFF(fileOFF);
  mesh.PrintNeighboringFacesOfFace(2);
  mesh.PrintNeighboringFacesOfVertex(2); 
#endif
#endif

  std::string outputFile= "/" + filename + ".obj"; 
  MEASURE_TIME(mesh.LoadOFF(fileOFF));
  MEASURE_TIME(mesh.ComputeNormals());

  MEASURE_TIME(mesh.SaveOBJ(outputFile));

  Application app(outputFile);
  app.run();
}