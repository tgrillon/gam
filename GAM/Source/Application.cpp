#include "Application.h"

Mesh make_grid( const int n= 10 )
{
  Mesh grid= Mesh(GL_LINES);
  
  // grille
  grid.color(White());
  for(int x= 0; x < n; x++)
  {
    float px= float(x) - float(n)/2 + .5f;
    grid.vertex(Point(px, 0, - float(n)/2 + .5f)); 
    grid.vertex(Point(px, 0, float(n)/2 - .5f));
  }

  for(int z= 0; z < n; z++)
  {
    float pz= float(z) - float(n)/2 + .5f;
    grid.vertex(Point(- float(n)/2 + .5f, 0, pz)); 
    grid.vertex(Point(float(n)/2 - .5f, 0, pz)); 
  }
  
  // axes XYZ
  grid.color(Red());
  grid.vertex(Point(0, .1, 0));
  grid.vertex(Point(1, .1, 0));
  
  grid.color(Green());
  grid.vertex(Point(0, .1, 0));
  grid.vertex(Point(0, 1, 0));
  
  grid.color(Blue());
  grid.vertex(Point(0, .1, 0));
  grid.vertex(Point(0, .1, 1));
  
  glLineWidth(2);
  
  return grid;
}

Mesh read_mesh(const std::string& filepath)
{
  return read_mesh(filepath.c_str());
}

Application::Application(const std::string& obj) : AppCamera(1024, 640), m_objFile(obj) {}

int Application::init()
{
  m_repere= make_grid(10);
  m_objet= read_mesh(std::string(DATA_DIR) + m_objFile);
  
  glClearColor(0.2f, 0.2f, 0.2f, 1.f);       
  
  glClearDepth(1.f);                         
  glDepthFunc(GL_LESS);                      
  glEnable(GL_DEPTH_TEST);                   

  return 0;  
}

int Application::quit()
{
  m_repere.release();
  m_objet.release();
  return 0;   // pas d'erreur
}

int Application::render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  draw(m_repere, /* model */ Identity(), camera());
  draw(m_objet, /* model */ Identity(), camera());
  
  return 1;
}