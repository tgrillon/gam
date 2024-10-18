#pragma once

#include "pch.h"

#include "Window.h"

const int MAX_FRAMES = 6;

class App
{
public:
    App(const int width, const int height, const int major = 3, const int minor = 3, const int samples = 1);
    virtual ~App();

    virtual int init();
    virtual int init_any() = 0;
    virtual int init_imgui();
    virtual int init_gl();
    virtual int quit();
    virtual int quit_any() = 0;
    virtual int quit_imgui();
    virtual int quit_sdl();

    virtual int update(const float time, const float delta) { return 0; }
    virtual int render() = 0;

    virtual int run();

private:
    bool event_imgui();

protected:
    virtual int prerender();
    virtual int postrender();

    void center_camera(const Mesh& mesh);

    std::pair<int, int> cpu_time() const { return {m_cpu_time / 1000, m_cpu_time % 1000}; }
    std::pair<int, int> gpu_time() const { return {int(m_frame_time / 1e6), int((m_frame_time / 1000) % 1000)}; }

    void vsync_off();

protected:
    bool m_exit{false};

    bool sync;

    Window m_window;
    Context m_context;

    Orbiter m_camera;

    ImGuiIO io;

    std::chrono::high_resolution_clock::time_point m_cpu_start;
    std::chrono::high_resolution_clock::time_point m_cpu_stop;

    GLuint m_time_query[MAX_FRAMES];
    GLint64 m_frame_time;
    int m_frame;
    int m_cpu_time;
};
