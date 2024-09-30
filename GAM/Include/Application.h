#pragma once

#include <string>

#include "wavefront.h"
#include "texture.h"

#include "orbiter.h"
#include "draw.h"        
#include "app_camera.h"      
#include "uniforms.h"

#include "utils.h"
#include "TMesh.h"

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

  inline void tmesh(const GAM::TMesh& mesh) { m_Mesh= mesh; }

protected:
  Mesh m_Object;
  Mesh m_Repere;

  GAM::TMesh m_Mesh;

  GLuint m_Program;
  GLuint m_HeatDiffusionTex;

  bool m_DrawNormals; 
  bool m_DrawCurvature; 
  bool m_DrawHeatDiffusion;
  bool m_DrawNormalColor;
  bool m_DrawSmoothNormal;

  std::string m_ObjFile; 
};
