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
    m_grid = make_grid(5);

    init_laplacian_demo();
    init_delaunay_demo();

    if (init_programs() < 0)
    {
        utils::error("in [init_progam]");
        return -1;
    }

    return 0;
}

int Viewer::init_programs()
{
    m_program = read_program(std::string(SHADER_DIR) + "/base.glsl");
    if (program_print_errors(m_program) < 0)
    {
        utils::error("in [read_program] for", std::string(SHADER_DIR) + "/base.glsl");
        return -1;
    }

    m_program_edges = read_program(std::string(SHADER_DIR) + "/edges.glsl");
    if (program_print_errors(m_program_edges) < 0)
    {
        utils::error("in [read_program] for", std::string(SHADER_DIR) + "/edges.glsl");
        return -1;
    }

    m_program_points = read_program(std::string(SHADER_DIR) + "/points.glsl");
    if (program_print_errors(m_program_points) < 0)
    {
        utils::error("in [read_program] for", std::string(SHADER_DIR) + "/points.glsl");
        return -1;
    }

    return 0;
}

int Viewer::init_laplacian_demo()
{
    m_laplacian.load_off("/cube.off");
    m_laplacian.vertex_value(0, 100);
    m_laplacian.smooth_normals();
    m_laplacian.curvature();
    m_object = m_laplacian.mesh();

    if (m_laplacian_demo)
    {
        center_camera(m_object);
    }

    m_heat_diffusion_tex = read_texture(0, std::string(DATA_DIR) + "/gradient.png");

    return 0;
}

int Viewer::init_delaunay_demo()
{
    // auto points = utils::read_point_set("/blue_noise.txt", 100., 100.);
    std::vector<Point> points;
    points.emplace_back(1.0, .0, .0);
    points.emplace_back(1.0, 1.0, .0);
    points.emplace_back(0.0, 1.0, .0);
    points.emplace_back(.0, .0, .0);
    points.emplace_back(.0, 2.0, .0);

    m_delaunay.insert_vertices(points);

    m_blue_noise = m_delaunay.mesh();

    if (m_delaunay_demo)
    {
        center_camera(m_blue_noise);
    }

    return 0;
}

int Viewer::handle_events()
{
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
                m_laplacian.curvature();
                m_object = m_laplacian.mesh(true);
                m_show_heat_diffusion = false;
            }
            m_show_curvature = !m_show_curvature;
        }

        if (key_state(SDLK_h))
        {
            clear_key_state(SDLK_h);
            if (m_show_curvature)
            {
                m_laplacian.reset_values();
                m_object = m_laplacian.mesh(false);
                m_show_curvature = false;
            }
            m_show_heat_diffusion = !m_show_heat_diffusion;
        }
    }

    return 0;
}

int Viewer::render()
{
    if (render_ui() < 0)
    {
        utils::error("in [render_ui]");
        return -1;
    }

    m_framebuffer.bind();
    glClearColor(m_clear_color[0], m_clear_color[1], m_clear_color[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
    glEnable(GL_DEPTH_TEST);

    if (render_any() < 0)
    {
        utils::error("in [render_any]");
        return -1;
    }

    m_framebuffer.unbind();

    return 1;
}

int Viewer::quit_any()
{
    m_grid.release();
    m_repere.release();
    m_object.release();
    m_blue_noise.release();
    glDeleteTextures(1, &m_heat_diffusion_tex);
    release_program(m_program);
    release_program(m_program_edges);
    return 0;
}

int Viewer::render_any()
{
    if (handle_events() < 0)
    {
        utils::error("in [handle_events]");
    }

    if (m_laplacian_demo)
        render_laplacian_demo();
    else if (m_delaunay_demo)
        render_delaunay_demo();

    return 0;
}

int Viewer::render_laplacian_demo()
{
    Transform model = Identity();
    Transform view = m_camera.view();
    Transform projection = m_camera.projection();

    Transform mvp = projection * view * model;
    Transform mv = model * view;

    glUseProgram(m_program);

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
        param.model(model).view(view).projection(projection);
        param.debug_normals(0.02 * m_camera.radius());
        param.draw(m_object);
    }
    else if (m_show_heat_diffusion)
    {
        m_laplacian.heat_diffusion(0.00001);
        m_object = m_laplacian.mesh(false);
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

        m_object.bind_vao();

        m_object.draw(m_program_edges, true, false, false, false, false);
    }

    if (m_show_points)
    {
        glUseProgram(m_program_points);

        program_uniform(m_program_points, "uMvpMatrix", mvp);
        program_uniform(m_program_points, "uPointSize", m_size_point);
        GLint location = glGetUniformLocation(m_program_points, "uPointColor");
        glUniform4fv(location, 1, &m_points_color[0]);

        m_object.bind_vao();

        glDrawArrays(GL_POINTS, 0, m_object.vertex_count());
    }

    return 0;
}

int Viewer::render_delaunay_demo()
{
    Transform model = Identity();
    Transform view = m_camera.view();
    Transform projection = m_camera.projection();

    Transform mvp = projection * view * model;
    Transform mv = model * view;

    glUseProgram(m_program);

    program_uniform(m_program, "uMvpMatrix", mvp);
    program_uniform(m_program, "uMvMatrix", mv);
    program_uniform(m_program, "uNormalMatrix", mv.normal());

    Point light = m_camera.position();
    program_uniform(m_program, "uLight", view(light));
    GLuint location = glGetUniformLocation(m_program, "uMeshColor");
    glUniform4fv(location, 1, &m_mesh_color[0]);
    if (m_blue_noise.triangle_count() > 0)
    {
        if (m_show_faces)
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0, 1.0);
            glDepthFunc(GL_LESS);

            m_blue_noise.draw(m_program, true, false, false, false, false);
            glDisable(GL_POLYGON_OFFSET_FILL);
        }
        if (m_show_edges)
        {
            glUseProgram(m_program_edges);

            glLineWidth(m_size_edge);
            program_uniform(m_program_edges, "uMvpMatrix", mvp);
            GLint location = glGetUniformLocation(m_program_edges, "uEdgeColor");
            glUniform4fv(location, 1, &m_edges_color[0]);

            m_blue_noise.draw(m_program_edges, true, false, false, false, false);
        }
    }

    if (m_show_points)
    {
        glUseProgram(m_program_points);

        program_uniform(m_program_points, "uMvpMatrix", mvp);
        program_uniform(m_program_points, "uPointSize", m_size_point);
        GLint location = glGetUniformLocation(m_program_points, "uPointColor");
        glUniform4fv(location, 1, &m_points_color[0]);

        m_blue_noise.bind_vao();

        glDrawArrays(GL_POINTS, 0, m_blue_noise.vertex_count());
    }

    return 0;
}

int Viewer::render_laplacian_params()
{
    ImGui::SeparatorText("LOAD MESH");
    ImGui::InputTextWithHint("Off name", "queen", &m_file_name);
    if (ImGui::Button("Load mesh", ImVec2(100, 25)))
    {
        m_laplacian.load_off("/" + m_file_name + ".off");
        m_obj_file = "/" + m_file_name + ".obj";
        m_laplacian.vertex_value(0, 100);
        m_laplacian.smooth_normals();
        m_laplacian.curvature();
        m_laplacian.save_obj(m_obj_file, true);
        m_object = read_mesh(std::string(OBJ_DIR) + m_obj_file);

        center_camera(m_object);
    }
    ImGui::SeparatorText("SCENE");

    // ImGui::InputText("Obj name", );
    ImGui::Checkbox("Show normals (n)", &m_show_normals);
    ImGui::Checkbox("Normal color (m)", &m_show_normal_color);
    if (ImGui::Checkbox("Curvature (w)", &m_show_curvature))
    {
        if (m_show_heat_diffusion)
        {
            m_laplacian.curvature();
            m_laplacian.save_obj(m_obj_file, true);
            m_object = read_mesh(std::string(OBJ_DIR) + m_obj_file);
            m_show_heat_diffusion = false;
        }
    }
    if (ImGui::Checkbox("Heat diffusion (h)", &m_show_heat_diffusion))
    {
        if (m_show_curvature)
        {
            m_laplacian.reset_values();
            m_laplacian.save_obj(m_obj_file);
            m_object = read_mesh(std::string(OBJ_DIR) + m_obj_file);
            m_show_curvature = false;
        }
    }

    return 0;
}

int Viewer::render_laplacian_stats()
{
    ImGui::Text("#vertices : %i", m_object.vertex_count());
    ImGui::Text("#triangles : %i", m_object.triangle_count());

    return 0;
}

int Viewer::render_delaunay_params()
{
    return 0;
}

int Viewer::render_delaunay_stats()
{
    ImGui::Text("#vertices : %i", m_blue_noise.vertex_count());
    ImGui::Text("#triangles : %i", m_blue_noise.triangle_count());

    return 0;
}

int Viewer::render_ui()
{
    ImGui::DockSpaceOverViewport();

    if (render_menu_bar() < 0)
    {
        utils::error("in [render_menu_bar]");
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

        render_demo_buttons();

        if (m_laplacian_demo)
            render_laplacian_params();
        else if (m_delaunay_demo)
            render_delaunay_params();

        // ImGui::Checkbox("Smooth normal (k)", &m_show_smooth_normal);
        ImGui::SeparatorText("GLOBAL");
        ImGui::Checkbox("Show vertices (v)", &m_show_points);
        ImGui::Checkbox("Show edges (e)", &m_show_edges);
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
        if (m_laplacian_demo)
            render_laplacian_stats();
        else if (m_delaunay_demo)
            render_delaunay_stats();
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

int Viewer::render_demo_buttons()
{
    ImVec2 sz = ImVec2(-FLT_MIN, 45.0f);

    //! Laplacian demo widget
    ImGui::BeginDisabled(m_laplacian_demo);
    if (ImGui::Button("Laplacian Demo", sz) && !m_laplacian_demo)
    {
        m_laplacian_demo = true;
        m_delaunay_demo = false;

        center_camera(m_object);
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip))
    {
        ImGui::SetTooltip(!m_laplacian_demo ? "Activate Laplacian demo." : "Laplacian demo is active.");
    }
    ImGui::EndDisabled();

    //! Delaunay demo widget
    ImGui::BeginDisabled(m_delaunay_demo);
    if (ImGui::Button("Delaunay Demo", sz) && !m_delaunay_demo)
    {
        m_laplacian_demo = false;
        m_delaunay_demo = true;

        center_camera(m_blue_noise);
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip))
    {
        ImGui::SetTooltip(!m_delaunay_demo ? "Activate Delaunay triangulation demo." : "Delaunay triangulation demo is active.");
    }
    ImGui::EndDisabled();

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
