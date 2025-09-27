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

	void set_infinite_z(Mesh& mesh, gam::TMesh& tmesh);

private:
	Mesh m_grid{};
	Mesh m_object{};
	Mesh m_repere{};
	Mesh m_object2{};

	gam::TMesh m_laplacian{};
	gam::TMesh m_delaunay{};

	GLuint m_program{};
	GLuint m_program_2{};
	GLuint m_program_edges{};
	GLuint m_program_points{};
	GLuint m_heat_diffusion_tex{};

	Framebuffer m_framebuffer{};

	std::string m_saved_file{};
	std::string m_file_name{};
	std::string m_file_cloud{};

	utils::Timer m_timer{};

	float m_clear_color[3]{ 0.678f, 0.686f, 0.878f };
	float m_mesh_color[4]{ 1.0, 1.0, 1.0, 1.0f };
	float m_edges_color[4]{ 0.0f, 0.0f, 1.0f, 1.0f };
	float m_points_color[4]{ 0.3f, 0.2f, 0.8f, 1.0f };

	float m_size_edge{ 1.0f };
	float m_size_point{ 15.0f };

	bool m_delaunay_demo{ false };
	bool m_laplacian_demo{ true };

	bool m_show_style_editor{ false };
	bool m_show_ui{ true };
	bool m_dark_theme{ true };
	bool m_need_update{ false };

	bool m_show_normals{ false };
	bool m_show_curvature{ false };
	bool m_show_heat_diffusion{ false };
	bool m_show_normal_color{ false };
	bool m_show_smooth_normal{ false };

	bool m_show_faces{ true };
	bool m_show_edges{ false };
	bool m_show_points{ false };

	bool m_show_infinite_faces{ false };
	bool m_shuffle{ true };

	int m_save_as_obj{ 1 };

	int m_dttms{ 0 }; //! Delaunay Triangulation Time (ms)
	int m_dttus{ 0 }; //! Delaunay Triangulation Time (us)

	float m_infinite_point_z{ 1.0 };
	float m_scale{ 1.0 };

	uint32_t m_point_count{ 0 };
	int m_insertion_count{ 1 };
	int m_loading_percentage{ 100 };
	std::vector<Point> m_points{};

	ImVec2 window_min{};
	ImVec2 window_max{};
};
