#pragma once

#include "App.h"
#include "Framebuffer.h"
#include "TMesh.h"
#include "Timer.h"
#include "Utils.h"

class Viewer : public App
{
public:
    Viewer();

    int init_any();
    int quit_any();

    int render();

private:
    int init_programs();
    int init_laplacian_demo();
    int init_delaunay_demo();

    int handle_events();

    int render_ui();
    int render_any();

    int render_laplacian_demo();
    int render_delaunay_demo();

    int render_laplacian_params();
    int render_laplacian_stats();
    int render_delaunay_params();
    int render_delaunay_stats();

    int render_demo_buttons();
    int render_menu_bar();

private:
    Mesh m_grid;
    Mesh m_object;
    Mesh m_repere;
    Mesh m_blue_noise;

    gam::TMesh m_laplacian;
    gam::TMesh m_delaunay;

    GLuint m_program;
    GLuint m_program_2;
    GLuint m_program_edges;
    GLuint m_program_points;
    GLuint m_heat_diffusion_tex;

    std::string m_obj_file;
    std::string m_file_name;
    std::string m_file_cloud;

    Framebuffer m_framebuffer;

    Timer m_timer;

    float m_clear_color[3]{0.678f, 0.686f, 0.878f};
    float m_mesh_color[4]{1.0, 1.0, 1.0, 1.0f};
    float m_edges_color[4]{0.0f, 0.0f, 1.0f, 1.0f};
    float m_points_color[4]{0.3f, 0.2f, 0.8f, 1.0f};

    float m_size_edge{1.0f};
    float m_size_point{15.0f};

    bool m_delaunay_demo{false};
    bool m_laplacian_demo{true};

    bool m_show_style_editor{false};
    bool m_show_ui{true};
    bool m_dark_theme{true};
    bool m_need_update{false};

    bool m_show_normals{false};
    bool m_show_curvature{false};
    bool m_show_heat_diffusion{false};
    bool m_show_normal_color{false};
    bool m_show_smooth_normal{false};

    bool m_show_faces{true};
    bool m_show_edges{false};
    bool m_show_points{false};

    bool m_show_infinite_faces{false};

    int m_dttms{0}; //! Delaunay Triangulation Time (ms)
    int m_dttus{0}; //! Delaunay Triangulation Time (us)

    int idx{3};
    std::vector<Point> points;

    ImVec2 window_min, window_max;
};