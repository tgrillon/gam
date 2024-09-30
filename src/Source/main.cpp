#include "TMesh.h"
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
#define RENDER_MESH

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
#ifdef RENDER_MESH
  mesh.load_off(fileOFF);
  mesh.vertex_value(0, 100);
  // mesh.vertex_value(1, -100);
  mesh.smooth_normals();
  mesh.curvature();
  mesh.save_obj(outputFile, true);
  Application app(outputFile);
  app.tmesh(mesh);
  app.run();
#endif
}