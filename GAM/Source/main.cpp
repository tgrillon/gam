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
        std::cout << "Time taken by " << #func << ": " << (duration.count() / 1000.0) << "ms" << "\n"; \
	

#define DEBUG
// #define CONNECTIVITY_TEST
#define HEAT_DIFFUSION
// #define SMOOTH_NORMAL
// #define CURVATURE

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
#if defined(SMOOTH_NORMAL) 
#ifdef DEBUG
  MEASURE_TIME(mesh.LoadOFF(fileOFF));
  MEASURE_TIME(mesh.ComputeNormals());
#ifdef CURVATURE
  MEASURE_TIME(mesh.ComputeCurvature());
#endif
  MEASURE_TIME(mesh.SaveOBJ(outputFile));
#else
  mesh.LoadOFF(fileOFF);
  mesh.ComputeNormals();
#ifdef CURVATURE
  mesh.ComputeCurvature()
#endif
  mesh.SaveOBJ(outputFile);
#endif
#endif

#if defined(HEAT_DIFFUSION) && !defined(CURVATURE) 
  int n= 100;
#ifdef DEBUG
  MEASURE_TIME(mesh.LoadOFF(fileOFF));
  MEASURE_TIME(mesh.ComputeNormals());
  MEASURE_TIME(while(n > 0)
  {
    mesh.HeatDiffusion(0, 100);
    n--;
  })
  MEASURE_TIME(mesh.SaveOBJ(outputFile));
#else
  mesh.LoadOFF(fileOFF);
  mesh.ComputeNormals()
  while(n > 0)
  {
    mesh.HeatDiffusion(0, 100);
    n--;
  }
  mesh.SaveOBJ(outputFile);
#endif
#endif

#if defined(SMOOTH_NORMAL) || defined(CURVATURE) || defined(HEAT_DIFFUSION)
  Application app(outputFile);
  app.run();
#endif
}