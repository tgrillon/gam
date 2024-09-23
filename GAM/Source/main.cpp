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
	

// #define DEBUG
// #define CONNECTIVITY_TEST
// #define HEAT_DIFFUSION
#define SMOOTH_NORMAL
#define CURVATURE

int main(int argc, char** argv)
{
  GAM::TMesh mesh; 
  std::string filename= argc > 1 ? argv[1] : "queen"; 
  std::string fileOFF= "/" + filename + ".off"; 
#ifdef CONNECTIVITY_TEST
#ifdef DEBUG
  MEASURE_TIME(mesh.load_off(fileOFF));
  MEASURE_TIME(mesh.print_neighboring_faces_of_face(2));
  MEASURE_TIME(mesh.print_neighboring_faces_of_vertex(2)); 
#else
  mesh.load_off(fileOFF);
  mesh.print_neighboring_faces_of_face(2);
  mesh.print_neighboring_faces_of_vertex(2); 
#endif
#endif

std::string outputFile= "/" + filename + ".obj"; 
#if defined(SMOOTH_NORMAL) 
#ifdef DEBUG
  MEASURE_TIME(mesh.load_off(fileOFF));
  MEASURE_TIME(mesh.smooth_normals());
#ifdef CURVATURE
  MEASURE_TIME(mesh.curvature());
#endif
  MEASURE_TIME(mesh.save_obj(outputFile));
#else
  mesh.load_off(fileOFF);
  mesh.smooth_normals();
#ifdef CURVATURE
  mesh.curvature();
#endif
  mesh.save_obj(outputFile);
#endif
#endif

#if defined(HEAT_DIFFUSION) && !defined(CURVATURE) 
  int n= 100;
#ifdef DEBUG
  MEASURE_TIME(mesh.load_off(fileOFF));
  MEASURE_TIME(mesh.smooth_normals());
  MEASURE_TIME(while(n > 0)
  {
    mesh.heat_diffusion(0, 3000, 100, -30);
    n--;
  })
  MEASURE_TIME(mesh.save_obj(outputFile));
#else
  mesh.load_off(fileOFF);
  mesh.smooth_normals();
  double dt= 1.0; 
  mesh.vertex_value(0, 100);
  mesh.vertex_value(40000, -100);
  while(n > 0)
  {
    mesh.heat_diffusion(dt, 0, 40000);
    n--;

    auto nghs= mesh.neighboring_vertices_of_vertex(0);
      utils::logln("vertex 0", ": ", mesh.vertex_value(0)); 
    for (auto v : nghs)
    {
      utils::logln("vertex ", v, ": ", mesh.vertex_value(v)); 
    }
    utils::logln();
  }
  mesh.save_obj(outputFile);
#endif
#endif

#if defined(SMOOTH_NORMAL) || defined(CURVATURE) || defined(HEAT_DIFFUSION)
  Application app(outputFile);
  app.run();
#endif
}