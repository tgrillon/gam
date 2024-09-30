#pragma once

#include <string>

#include "uniforms.h"
#include "draw.h"
#include "mesh.h"
#include "wavefront.h"

#include "App.h"
#include "Framebuffer.h"
#include "TMesh.h"
#include "utils.h"

class Viewer : public App
{
public:
  Viewer(const std::string& obj);

  int init_any();
  int quit_any();

  int render();

private:
  int render_ui();
  int render_any();

  int render_menu_bar(); 

private: 
  Mesh m_object;
  Mesh m_repere;

  GLuint m_program;
  GLuint m_heat_diffusion_tex;

  std::string m_obj_file;

  Framebuffer m_framebuffer; 

  bool m_show_style_editor{false};
  bool m_show_ui {true};
  bool m_need_update{false}; 

  bool m_show_normals{false}; 
  bool m_show_curvature{false}; 
  bool m_show_heat_diffusion{false};
  bool m_show_normal_color{false};
  bool m_show_smooth_normal{false};
};