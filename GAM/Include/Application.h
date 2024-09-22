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
  Mesh m_Object;
  Mesh m_Repere;

  unsigned int m_Program;

  bool m_DrawNormals; 
  bool m_DrawCurvature; 

  std::string m_ObjFile; 
};
