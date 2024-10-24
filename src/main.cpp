#include "Viewer.h"

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
// #define TEST
#define RENDER_MESH

int main(int argc, char** argv)
{
  gam::TMesh mesh; 
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
#ifdef RENDER_MESH
  Viewer app;
  app.run();
#endif

}