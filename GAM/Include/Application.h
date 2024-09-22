#pragma once

#include <string>

#include "wavefront.h"
#include "texture.h"

#include "orbiter.h"
#include "draw.h"        
#include "app_camera.h"      

class Application : public AppCamera
{
public:
  Application(const std::string& obj);
  
  // creation des objets de l'application
  int init();

  // destruction des objets de l'application
  int quit();
  
  // dessiner une nouvelle image
  int render();

protected:
  Mesh m_objet;
  Mesh m_cube;
  Mesh m_robot;
  Mesh m_repere;

  std::string m_objFile; 
};
