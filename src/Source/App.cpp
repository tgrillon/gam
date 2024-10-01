#include "App.h"

App::App(const int width, const int height, const int major, const int minor, const int samples) 
{
  m_window= create_window(width, height, major, minor, samples);
  m_context= create_context(m_window);

  // requetes pour mesurer le temps gpu
  m_frame= 0;
  glGenQueries(MAX_FRAMES, m_time_query);
  
  // initialise les queries, plus simple pour demarrer 
  for(int i= 0; i < MAX_FRAMES; i++)
  {
    glBeginQuery(GL_TIME_ELAPSED, m_time_query[i]); 
    glEndQuery(GL_TIME_ELAPSED);
  }
}

App::~App()
{
  if(m_context) release_context(m_context);
  if(m_window) release_window(m_window);
  glDeleteQueries(MAX_FRAMES, m_time_query);
}

int App::prerender()
{
  #ifndef GK_RELEASE
    // verifie que la requete est bien dispo sans attente...
    {
      GLuint ready= GL_FALSE;
      glGetQueryObjectuiv(m_time_query[m_frame], GL_QUERY_RESULT_AVAILABLE, &ready);
      if(ready != GL_TRUE)
        printf("[oops] wait query, frame %d...\n", m_frame);
    }
  #endif    
  // recupere la mesure precedente...
  m_frame_time= 0;
  glGetQueryObjecti64v(m_time_query[m_frame], GL_QUERY_RESULT, &m_frame_time);
  
  // prepare la mesure de la frame courante...
  glBeginQuery(GL_TIME_ELAPSED, m_time_query[m_frame]);
  
  // mesure le temps d'execution du draw pour le cpu
  // utilise std::chrono pour mesurer le temps cpu 
  m_cpu_start= std::chrono::high_resolution_clock::now();

  // recupere les mouvements de la souris
  int mx, my;
  unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);
  int mousex, mousey;
  SDL_GetMouseState(&mousex, &mousey);
  
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  if (!io.WantCaptureMouse) 
  {
    // deplace la camera
    if(mb & SDL_BUTTON(1))
      m_camera.rotation(mx, my);      // tourne autour de l'objet
    else if(mb & SDL_BUTTON(3))
      m_camera.translation((float) mx / (float) window_width(), (float) my / (float) window_height()); // deplace le point de rotation
    else if(mb & SDL_BUTTON(2))
      m_camera.move(mx);           // approche / eloigne l'objet
    
    SDL_MouseWheelEvent wheel= wheel_event();
    if(wheel.y != 0)
    {
      clear_wheel_event();
      m_camera.move(8.f * wheel.y);  // approche / eloigne l'objet
    }
  }

  const char *orbiter_filename= "app_orbiter.txt";
  // copy / export / write orbiter
  if(key_state('c'))
  {
    clear_key_state('c');
    m_camera.write_orbiter(orbiter_filename);
  }
  // paste / read orbiter
  if(key_state('v'))
  {
    clear_key_state('v');
    
    Orbiter tmp;
    if(tmp.read_orbiter(orbiter_filename) < 0)
      // ne pas modifer la camera en cas d'erreur de lecture...
      tmp= m_camera;
    
    m_camera= tmp;
  }
  
  // screenshot
  if(key_state('s'))
  {
    static int calls= 1;
    clear_key_state('s');
    screenshot("app", calls++);
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
  
  // appelle la fonction update() de la classe derivee
  return update(global_time(), delta_time());
}

int App::postrender()
{
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Update and Render additional Platform Windows
  // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
    SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
  }

  m_cpu_stop= std::chrono::high_resolution_clock::now();
  m_cpu_time= std::chrono::duration_cast<std::chrono::microseconds>(m_cpu_stop - m_cpu_start).count(); 
  
  glEndQuery(GL_TIME_ELAPSED);
  
  // selectionne une requete pour la frame suivante...
  m_frame= (m_frame + 1) % MAX_FRAMES;

  return 0;
}


int App::run()
{
  if(init() < 0)
    return -1;
  
  glViewport(0, 0, window_width(), window_height());
  
  while(events(m_window) && !m_exit)
  {
    if(prerender() < 0)
      break;
    
    if(render() < 1) 
      break;

    if(postrender() < 0)
      break;
    
    SDL_GL_SwapWindow(m_window);
    
    if(sync)
      glFinish();
  }
  
  if(quit() < 0)
    return -1;
  
  return 0;
}

bool App::event_imgui()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    // Forward to Imgui
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT
      || (event.type == SDL_WINDOWEVENT
      && event.window.event == SDL_WINDOWEVENT_CLOSE
      && event.window.windowID == SDL_GetWindowID(m_window))) {
      return false;
    }
  }

  return true;
}

void App::vsync_off()
{
  // desactive vsync pour les mesures de temps
  SDL_GL_SetSwapInterval(0);
  printf("sync + vsync  OFF...\n");
  
  // desactive aussi la synchro cpu / gpu...
  sync= false;
}

int App::init()
{
  if (init_imgui() < 0) return -1;
  if (init_any() < 0) return -1;
  if (init_gl() < 0) return -1;

  return 0;
}

int App::init_imgui()
{
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  // ImGui::LoadIniSettingsFromMemory((std::string(CMAKE_SOURCE_DIR) + "/imgui.ini").c_str());
  // io.WantSaveIniSettings= true; 

  // Setup Dear ImGui style
  ImGui::StyleColorsClassic();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
  ImGui_ImplOpenGL3_Init("#version 330");

  return 0;
}

int App::init_gl()
{
  glClearColor(0.2f, 0.2f, 0.2f, 1.f);      
  
  glClearDepth(1.f);                        
  glDepthFunc(GL_LESS);                     
  glEnable(GL_DEPTH_TEST);                  

  return 0;
}

int App::quit()
{
  if (quit_any() < 0) return -1;
  if (quit_imgui() < 0) return -1;
  if (quit_sdl() < 0) return -1;

  return 0;
}

int App::quit_sdl()
{
  // Cleanup SDL
  SDL_GL_DeleteContext(m_context);
  SDL_DestroyWindow(m_window);
  SDL_Quit();

  return 0;
}

int App::quit_imgui()
{
  ImGui::SaveIniSettingsToDisk((std::string(CMAKE_SOURCE_DIR) + "/imgui.ini").c_str());
  // Cleanup ImGui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  return 0;
}
