#include "Application.h"

Mesh read_mesh(const std::string& filepath)
{
  return read_mesh(filepath.c_str());
}

GLuint read_texture(const int location, const std::string& filepath)
{
  return read_texture(location, filepath.c_str());
}

GLuint read_program(const std::string& filepath)
{
  return read_program(filepath.c_str());
}

Application::Application(const std::string& obj) : AppCamera(1024, 640), m_ObjFile(obj) {
}

int Application::init()
{
  m_Object= read_mesh(std::string(OBJ_DIR) + m_ObjFile);
  
  glClearColor(0.1, 0.1, 0.1, 1.f); 
  
  glClearDepth(1.f);                         
  glDepthFunc(GL_LESS);                      
  glEnable(GL_DEPTH_TEST);  

  Point pmin, pmax; 
  m_Object.bounds(pmin, pmax);
  camera().lookat(pmin, pmax);

  m_DrawNormals= false; 
  m_DrawCurvature= false; 
  m_DrawHeatDiffusion= false; 
  m_DrawNormalColor= false; 
  m_DrawSmoothNormal= false; 

  m_HeatDiffusionTex= read_texture(0, std::string(DATA_DIR) + "/gradient.png");

  m_Program= read_program(std::string(SHADER_DIR) + "/gam.glsl");
  program_print_errors(m_Program);

  return 0;  
}

int Application::quit()
{
  m_Repere.release();
  m_Object.release();
  glDeleteTextures(1, &m_HeatDiffusionTex);
  release_program(m_Program);
  return 0;   // pas d'erreur
}

int Application::render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(m_Program);
  
  Transform tf= Scale(camera().radius() * 0.1);

  if (key_state(SDLK_n)) 
  {
    clear_key_state(SDLK_n);
    m_DrawNormals= !m_DrawNormals; 
  }

  if (key_state(SDLK_k)) 
  {
    clear_key_state(SDLK_k);
    m_DrawSmoothNormal= !m_DrawSmoothNormal; 
  }

  if (key_state(SDLK_m))
  {
    clear_key_state(SDLK_m);
    m_DrawNormalColor= !m_DrawNormalColor;
  } 

  if (key_state(SDLK_w))
  {
    clear_key_state(SDLK_w);
    if (m_DrawHeatDiffusion)
    {
      m_Mesh.curvature();
      m_Mesh.save_obj(m_ObjFile, true);
      m_Object= read_mesh(std::string(OBJ_DIR) + m_ObjFile);
      m_DrawHeatDiffusion= false; 
    }
    m_DrawCurvature= !m_DrawCurvature; 
  }

  if (key_state(SDLK_h))
  {
    clear_key_state(SDLK_h);
    if (m_DrawCurvature)
    {
      m_Mesh.reset_values();
      m_Mesh.save_obj(m_ObjFile);
      m_Object= read_mesh(std::string(OBJ_DIR) + m_ObjFile);
      m_DrawCurvature= false; 
    }
    m_DrawHeatDiffusion= !m_DrawHeatDiffusion; 
  }

  Transform model= Identity();
  Transform view= m_camera.view();
  Transform projection= m_camera.projection(window_width(), window_height(), 45);
  
  // . composer les transformations : model, view et projection
  Transform mvp= projection * view * model;
  Transform mv= model * view;
  
  // . parametrer le shader program :
  //   . transformation : la matrice declaree dans le vertex shader s'appelle mvpMatrix
  program_uniform(m_Program, "u_MvpMatrix", mvp);
  program_uniform(m_Program, "u_MvMatrix", mv);
  program_uniform(m_Program, "u_NormalMatrix", mv.normal());

  Point light= camera().position();
  program_uniform(m_Program, "u_Light", view(light));
  program_uniform(m_Program, "u_DrawCurvature", m_DrawCurvature ? 1 : 0);
  program_uniform(m_Program, "u_DrawHeatDiffusion", m_DrawHeatDiffusion ? 1 : 0);
  program_uniform(m_Program, "u_DrawNormalColor", m_DrawNormalColor ? 1 : 0);
  program_uniform(m_Program, "u_DrawSmoothNormal", m_DrawSmoothNormal ? 1 : 0);

  if (m_DrawNormals)
  {
    DrawParam param;
    param.model(Identity()).view(view).projection(projection);
    param.debug_normals(0.02*camera().radius());
    param.draw(m_Object);
  }
  else if (m_DrawHeatDiffusion)
  {
    m_Mesh.heat_diffusion(0.00001);
    m_Mesh.save_obj(m_ObjFile);
    m_Object= read_mesh(std::string(OBJ_DIR) + m_ObjFile);
    program_use_texture(m_Program, "u_HeatDiffusionTex", 0, m_HeatDiffusionTex);
    m_Object.draw(m_Program, true, true, true, false, false);
  }
  else 
  {
    m_Object.draw(m_Program, true, true, true, false, false);
  }

  return 1;
} 