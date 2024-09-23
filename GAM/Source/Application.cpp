#include "Application.h"

#include "uniforms.h"

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
  m_Repere= make_grid(10);
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

  m_HeatDiffusionTex= read_texture(0, std::string(DATA_DIR) + "/gradient.jpg");

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
  // draw(m_Repere, /* model */ tf, camera());

  if (key_state(SDLK_LCTRL) && key_state(SDLK_n)) 
  {
    clear_key_state(SDLK_n);
    m_DrawNormals= !m_DrawNormals; 
    m_DrawCurvature= false; 
  }

  if (key_state(SDLK_LCTRL) && key_state(SDLK_w))
  {
    clear_key_state(SDLK_w);
    m_DrawCurvature= !m_DrawCurvature; 
    m_DrawNormals= false; 
  }

  // Transform view= camera().view();
  // Transform projection= camera().projection(window_width(), window_height(), 45);
  // if (m_DrawNormals) param.debug_normals(0.02);
  // if (m_DrawCurvature) 
  // {

  //   param.debug_texcoords();
  // }
  // param.draw(m_Object);

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


  if (m_DrawNormals)
  {
    DrawParam param;
    param.model(Identity()).view(view).projection(projection);
    param.debug_normals(0.02);
    param.draw(m_Object);
  }
  if (m_DrawHeatDiffusion)
  {
    program_use_texture(m_Program, "u_HeatDiffusionTex", 0, m_HeatDiffusionTex);
    program_uniform(m_Program, "u_DrawHeatDiffusion", m_DrawHeatDiffusion ? 1 : 0);
    m_Object.draw(m_Program, true, true, true, false, false);
  }
  else 
  {
    m_Object.draw(m_Program, true, true, true, false, false);
  }
  
  return 1;
} 