set(IMGUI_SOURCE_DIR "./Source")
set(IMGUI_INCLUDE_DIR "./Include")
add_library(imgui STATIC ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
                         ${IMGUI_SOURCE_DIR}/imgui_draw.cpp                                               
                         ${IMGUI_SOURCE_DIR}/imgui_impl_sdl2.cpp                         
                         ${IMGUI_SOURCE_DIR}/imgui_impl_opengl3.cpp                         
                         ${IMGUI_SOURCE_DIR}/imgui_tables.cpp                         
                         ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp                         
                         ${IMGUI_SOURCE_DIR}/imgui.cpp
                         ${IMGUI_SOURCE_DIR}/imgui_stdlib.cpp
                         ${IMGUI_INCLUDE_DIR}/imconfig.h                         
                         ${IMGUI_INCLUDE_DIR}/imgui_impl_sdl2.h                         
                         ${IMGUI_INCLUDE_DIR}/imgui_impl_opengl3.h                         
                         ${IMGUI_INCLUDE_DIR}/imgui_impl_opengl3_loader.h                         
                         ${IMGUI_INCLUDE_DIR}/imgui_internal.h                         
                         ${IMGUI_INCLUDE_DIR}/imgui.h                         
                         ${IMGUI_INCLUDE_DIR}/imgui_stdlib.h                         
                         ${IMGUI_INCLUDE_DIR}/imstb_rectpack.h                         
                         ${IMGUI_INCLUDE_DIR}/imstb_textedit.h                         
                         ${IMGUI_INCLUDE_DIR}/imstb_truetype.h)

target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR})
target_link_libraries(imgui PUBLIC SDL2)