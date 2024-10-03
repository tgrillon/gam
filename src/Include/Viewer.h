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

  inline void tmesh(const gam::TMesh& mesh) { m_tmesh= mesh; }

private:
  int render_ui();
  int render_any();

  int render_menu_bar(); 

private: 
  Mesh m_object;
  Mesh m_repere;

  gam::TMesh m_tmesh; 

  GLuint m_program;
  GLuint m_heat_diffusion_tex;

  std::string m_obj_file;
  std::string m_file_name;

  Framebuffer m_framebuffer; 

  float m_clear_color[3]{0.678f, 0.686f, 0.878f}; 
  float m_mesh_color[4]{1.0f, 1.0f, 1.0f, 1.0f}; 
  float m_light_color[4]{1.0f, 1.0f, 1.0f, 1.0f}; 

  bool m_show_style_editor{false};
  bool m_show_ui {true};
  bool m_dark_theme {true};
  bool m_need_update{false}; 

  bool m_show_normals{false}; 
  bool m_show_curvature{false}; 
  bool m_show_heat_diffusion{false};
  bool m_show_normal_color{false};
  bool m_show_smooth_normal{false};
};