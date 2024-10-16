#include "Viewer.h"

Mesh make_grid(const int n = 10)
{
    Mesh grid = Mesh(GL_LINES);

    // grille
    grid.color(White());
    for (int x = 0; x < n; x++)
    {
        float px = float(x) - float(n) / 2 + .5f;
        grid.vertex(Point(px, 0, -float(n) / 2 + .5f));
        grid.vertex(Point(px, 0, float(n) / 2 - .5f));
    }

    for (int z = 0; z < n; z++)
    {
        float pz = float(z) - float(n) / 2 + .5f;
        grid.vertex(Point(-float(n) / 2 + .5f, 0, pz));
        grid.vertex(Point(float(n) / 2 - .5f, 0, pz));
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

Mesh read_mesh(const std::string &filepath)
{
    return read_mesh(filepath.c_str());
}

GLuint read_texture(const int location, const std::string &filepath)
{
    return read_texture(location, filepath.c_str());
}

GLuint read_program(const std::string &filepath)
{
    return read_program(filepath.c_str());
}

Viewer::Viewer() : App(1024, 640), m_framebuffer(window_width(), window_height()), m_obj_file("/queen.obj")
{
    m_camera.projection(window_width(), window_height(), 45);
}

int Viewer::init_any()
{
    m_tmesh.load_off("/cube.off");
    m_tmesh.vertex_value(0, 100);
    m_tmesh.smooth_normals();
    m_tmesh.curvature();
    // int resolution = 15;
    // double angleStep = 2 * M_PI / resolution;
    // for (int i= 0; i < resolution; ++i)
    // {
    //     double theta= angleStep * i;
    //     m_tmesh.insert_vertex({0.5+0.35*cos(theta), 0.5+0.35*sin(theta), 0.});
    // }

    // m_tmesh.flip_edge(0, 2);
    // m_tmesh.insert_vertex({0.66, 0.33, 0.0});
    // m_tmesh.insert_vertex({0.6, 0.6, 0.0});
    m_tmesh.insert_vertex({0.5, 1.0, 0.5});
    m_obj_file = "/cube.obj";
    m_tmesh.save_obj(m_obj_file, true);

    m_object = read_mesh(std::string(OBJ_DIR) + m_obj_file);

    Point pmin, pmax;
    m_object.bounds(pmin, pmax);
    m_camera.lookat(pmin, pmax);

    m_heat_diffusion_tex = read_texture(0, std::string(DATA_DIR) + "/gradient.png");

    m_program = read_program(std::string(SHADER_DIR) + "/base.glsl");
    program_print_errors(m_program);

    m_program_edges = read_program(std::string(SHADER_DIR) + "/edges.glsl");
    program_print_errors(m_program_edges);

    m_program_points = read_program(std::string(SHADER_DIR) + "/points.glsl");
    program_print_errors(m_program_points);

    return 0;
}

int Viewer::render()
{
    if (render_ui() < 0)
    {
        utils::error("Error with the UI rendering!");
        return -1;
    }

    m_framebuffer.bind();
    glClearColor(m_clear_color[0], m_clear_color[1], m_clear_color[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
    glEnable(GL_DEPTH_TEST);

    if (render_any() < 0)
    {
        utils::error("Error with the geometry rendering!");
        return -1;
    }

    m_framebuffer.unbind();

    return 1;
}

int Viewer::quit_any()
{
    m_repere.release();
    m_object.release();
    glDeleteTextures(1, &m_heat_diffusion_tex);
    release_program(m_program);
    release_program(m_program_edges);
    return 0;
}

int Viewer::render_any()
{
    glUseProgram(m_program);

    Transform tf = Scale(m_camera.radius() * 0.1);

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    if (!io.WantCaptureKeyboard && !io.WantCaptureMouse)
    {

        if (key_state(SDLK_n))
        {
            clear_key_state(SDLK_n);
            m_show_normals = !m_show_normals;
        }

        if (key_state(SDLK_e))
        {
            clear_key_state(SDLK_e);
            m_show_edges = !m_show_edges;
        }

        if (key_state(SDLK_v))
        {
            clear_key_state(SDLK_v);
            m_show_points = !m_show_points;
        }

        if (key_state(SDLK_f))
        {
            clear_key_state(SDLK_f);
            m_show_faces = !m_show_faces;
        }

        if (key_state(SDLK_k))
        {
            clear_key_state(SDLK_k);
            m_show_smooth_normal = !m_show_smooth_normal;
        }

        if (key_state(SDLK_m))
        {
            clear_key_state(SDLK_m);
            m_show_normal_color = !m_show_normal_color;
        }

        if (key_state(SDLK_w))
        {
            clear_key_state(SDLK_w);
            if (m_show_heat_diffusion)
            {
                m_tmesh.curvature();
                m_tmesh.save_obj(m_obj_file, true);
                m_object = read_mesh(std::string(OBJ_DIR) + m_obj_file);
                m_show_heat_diffusion = false;
            }
            m_show_curvature = !m_show_curvature;
        }

        if (key_state(SDLK_h))
        {
            clear_key_state(SDLK_h);
            if (m_show_curvature)
            {
                m_tmesh.reset_values();
                m_tmesh.save_obj(m_obj_file);
                m_object = read_mesh(std::string(OBJ_DIR) + m_obj_file);
                m_show_curvature = false;
            }
            m_show_heat_diffusion = !m_show_heat_diffusion;
        }
    }

    Transform model = Identity();
    Transform view = m_camera.view();
    Transform projection = m_camera.projection();

    // . composer les transformations : model, view et projection
    Transform mvp = projection * view * model;
    Transform mv = model * view;

    // . parametrer le shader program :
    //   . transformation : la matrice declaree dans le vertex shader s'appelle mvpMatrix
    program_uniform(m_program, "uMvpMatrix", mvp);
    program_uniform(m_program, "uMvMatrix", mv);
    program_uniform(m_program, "uNormalMatrix", mv.normal());

    Point light = m_camera.position();
    program_uniform(m_program, "uLight", view(light));
    GLuint location = glGetUniformLocation(m_program, "uMeshColor");
    glUniform4fv(location, 1, &m_mesh_color[0]);
    program_uniform(m_program, "uShowCurvature", m_show_curvature ? 1 : 0);
    program_uniform(m_program, "uShowHeatDiffusion", m_show_heat_diffusion ? 1 : 0);
    program_uniform(m_program, "uShowNormalColor", m_show_normal_color ? 1 : 0);
    program_uniform(m_program, "uShowSmoothNormal", m_show_smooth_normal ? 1 : 0);

    if (m_show_normals)
    {
        DrawParam param;
        param.model(Identity()).view(view).projection(projection);
        param.debug_normals(0.02 * m_camera.radius());
        param.draw(m_object);
    }
    else if (m_show_heat_diffusion)
    {
        m_tmesh.heat_diffusion(0.00001);
        m_tmesh.save_obj(m_obj_file);
        m_object = read_mesh(std::string(OBJ_DIR) + m_obj_file);
        program_use_texture(m_program, "uHeatDiffusionTex", 0, m_heat_diffusion_tex);
        m_object.draw(m_program, true, true, true, false, false);
    }
    else if (m_show_faces)
    {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 1.0);
        glDepthFunc(GL_LESS);

        m_object.draw(m_program, true, true, true, false, false);
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    if (m_show_edges)
    {
        glUseProgram(m_program_edges);

        glLineWidth(m_size_edge);
        program_uniform(m_program_edges, "uMvpMatrix", mvp);
        GLint location = glGetUniformLocation(m_program_edges, "uEdgeColor");
        glUniform4fv(location, 1, &m_edges_color[0]);

        m_object.draw(m_program_edges, true, false, false, false, false);
    }

    if (m_show_points)
    {
        glUseProgram(m_program_points);

        program_uniform(m_program_points, "uMvpMatrix", mvp);
        program_uniform(m_program_points, "uPointSize", m_size_point);
        GLint location = glGetUniformLocation(m_program_points, "uPointColor");
        glUniform4fv(location, 1, &m_points_color[0]);

        glDrawArrays(GL_POINTS, 0, m_object.vertex_count());
    }

    return 0;
}

int Viewer::render_ui()
{
    ImGui::DockSpaceOverViewport();

    if (render_menu_bar() < 0)
    {
        utils::error("Error with the menu bar rendering!");
        return -1;
    }

    ImGui::Begin("Scene");

    if (ImGui::IsWindowHovered())
    {
        ImGuiIO &io = ImGui::GetIO();
        (void)io;

        io.WantCaptureMouse = false;
        io.WantCaptureKeyboard = false;
    }

    // we access the ImGui window size
    const float window_width = ImGui::GetContentRegionAvail().x;
    const float window_height = ImGui::GetContentRegionAvail().y;

    // we rescale the framebuffer to the actual window size here and reset the glViewport
    m_framebuffer.rescale(window_width, window_height);
    glViewport(0, 0, window_width, window_height);
    m_camera.projection(window_width, window_height, 45.f);

    // we get the screen position of the window
    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImGui::GetWindowDrawList()->AddImage(
        (void *)m_framebuffer.texture_id(),
        ImVec2(pos.x, pos.y),
        ImVec2(pos.x + window_width, pos.y + window_height),
        ImVec2(0, 1),
        ImVec2(1, 0));

    ImGui::End();

    if (m_show_ui)
    {
        ImGui::Begin("Parameters");
        ImGui::SeparatorText("LOAD MESH");
        ImGui::InputTextWithHint("Off name", "queen", &m_file_name);
        if (ImGui::Button("Load mesh", ImVec2(100, 25)))
        {
            m_tmesh.load_off("/" + m_file_name + ".off");
            m_obj_file = "/" + m_file_name + ".obj";
            m_tmesh.vertex_value(0, 100);
            m_tmesh.smooth_normals();
            m_tmesh.curvature();
            m_tmesh.save_obj(m_obj_file, true);
            m_object = read_mesh(std::string(OBJ_DIR) + m_obj_file);

            Point pmin, pmax;
            m_object.bounds(pmin, pmax);
            m_camera.lookat(pmin, pmax);
        }
        ImGui::SeparatorText("SCENE");

        // ImGui::InputText("Obj name", );
        ImGui::Checkbox("Show normals (n)", &m_show_normals);
        ImGui::Checkbox("Normal color (m)", &m_show_normal_color);
        if (ImGui::Checkbox("Curvature (w)", &m_show_curvature))
        {
            if (m_show_heat_diffusion)
            {
                m_tmesh.curvature();
                m_tmesh.save_obj(m_obj_file, true);
                m_object = read_mesh(std::string(OBJ_DIR) + m_obj_file);
                m_show_heat_diffusion = false;
            }
        }
        if (ImGui::Checkbox("Heat diffusion (h)", &m_show_heat_diffusion))
        {
            if (m_show_curvature)
            {
                m_tmesh.reset_values();
                m_tmesh.save_obj(m_obj_file);
                m_object = read_mesh(std::string(OBJ_DIR) + m_obj_file);
                m_show_curvature = false;
            }
        }
        ImGui::Checkbox("Smooth normal (k)", &m_show_smooth_normal);
        ImGui::Checkbox("Show edges (e)", &m_show_edges);
        ImGui::SameLine();
        ImGui::Checkbox("Show faces (f)", &m_show_faces);
        ImGui::SliderFloat("Edges size", &m_size_edge, 1, 10, "%.2f");
        ImGui::SliderFloat("Points size", &m_size_point, 1, 30, "%.2f");
        ImGui::SeparatorText("COLOR");
        ImGui::ColorPicker3("Background", &m_clear_color[0]);
        ImGui::ColorPicker3("Mesh", &m_mesh_color[0]);
        ImGui::ColorPicker3("Edges", &m_edges_color[0]);
        ImGui::ColorPicker3("Points", &m_points_color[0]);
        ImGui::End();

        ImGui::Begin("Statistiques");
        ImGui::SeparatorText("PERFORMANCES");
        auto [cpums, cpuus] = cpu_time();
        auto [gpums, gpuus] = gpu_time();
        ImGui::Text("fps : %.2f ", (1000.f / delta_time()));
        ImGui::Text("cpu : %i ms %i us ", cpums, cpuus);
        ImGui::Text("gpu : %i ms %i us", gpums, gpuus);
        ImGui::Text("total : %.2f ms", delta_time());
        ImGui::SeparatorText("GEOMETRY");
        ImGui::Text("#vertices : %i", m_object.vertex_count() / 3);
        ImGui::Text("#triangles : %i", m_object.triangle_count());
        ImGui::End();
    }

    if (m_show_style_editor)
    {
        ImGui::Begin("Dear ImGui Style Editor", &m_show_style_editor);
        ImGui::ShowStyleEditor();
        ImGui::End();
    }

    ImGui::Render();

    return 0;
}

int Viewer::render_menu_bar()
{
    ImGui::DockSpace(ImGui::GetID("DockSpace"));

    if (ImGui::BeginMainMenuBar())
    {
        // ImGui::MenuItem("Style Editor", NULL, &m_show_style_editor);
        ImGui::MenuItem("Exit", NULL, &m_exit);
        ImGui::MenuItem(m_show_ui ? "Hide UI" : "Show UI", NULL, &m_show_ui);
        if (ImGui::MenuItem(m_dark_theme ? "Light Theme" : "Dark Theme", NULL, &m_dark_theme))
        {
            if (m_dark_theme)
            {
                ImGui::StyleColorsDark();
            }
            else
            {
                ImGui::StyleColorsLight();
            }
        }

        ImGui::EndMainMenuBar();
    }

    // ImGui::ShowDemoWindow();

    return 0;
}
