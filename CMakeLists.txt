cmake_minimum_required (VERSION 3.26)

project(gam)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GKIT_DIR "/home/tgrillon/dev/cxx/gkit2light" CACHE PATH "Path to gkit source files")
message(STATUS "Gkit directory set to: ${GKIT_DIR}")
set(DATA_DIR "${CMAKE_SOURCE_DIR}/data" CACHE PATH "Path to the data directory")
message(STATUS "Data directory set to: ${DATA_DIR}")
set(OFF_DIR "${DATA_DIR}/off" CACHE PATH "Path to the off files")
message(STATUS "Off files directory set to: ${OFF_DIR}")
set(OBJ_DIR "${DATA_DIR}/obj" CACHE PATH "Path to the obj files")
message(STATUS "Obj files directory set to: ${OBJ_DIR}")
set(SHADER_DIR "${DATA_DIR}/shaders" CACHE PATH "Path to the shader files")
message(STATUS "Shader files directory set to: ${SHADER_DIR}")

# Fetching gkit library 
file(GLOB gkit_files "${GKIT_DIR}/src/gKit/*.cpp")
add_library(gkit STATIC ${gkit_files})
target_include_directories(gkit PUBLIC "${GKIT_DIR}/src/gKit")
target_link_libraries(gkit PUBLIC GL GLEW SDL2 SDL2_image)

# Fetching imgui library 
set(IMGUI_SOURCE_DIR "${CMAKE_SOURCE_DIR}/vendor/imgui/Source")
set(IMGUI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/vendor/imgui/Include")
add_library(imgui STATIC ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
                         ${IMGUI_SOURCE_DIR}/imgui_draw.cpp                                               
                         ${IMGUI_SOURCE_DIR}/imgui_impl_sdl2.cpp                         
                         ${IMGUI_SOURCE_DIR}/imgui_impl_opengl3.cpp                         
                         ${IMGUI_SOURCE_DIR}/imgui_tables.cpp                         
                         ${IMGUI_SOURCE_DIR}/imgui_stdlib.cpp                         
                         ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp                         
                         ${IMGUI_SOURCE_DIR}/imgui.cpp
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

set(MAIN_DIR "${CMAKE_SOURCE_DIR}/src/")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/Source")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/Include")
add_executable(${PROJECT_NAME} ${MAIN_DIR}/main.cpp 
                               ${SOURCE_DIR}/TMesh.cpp
                               ${SOURCE_DIR}/geometry.cpp
                               ${SOURCE_DIR}/Viewer.cpp
                               ${SOURCE_DIR}/Window.cpp
                               ${SOURCE_DIR}/App.cpp
                               ${SOURCE_DIR}/Framebuffer.cpp
                               ${SOURCE_DIR}/Delaunay.cpp
                               ${INCLUDE_DIR}/utils.h
                               ${INCLUDE_DIR}/TMesh.h
                               ${INCLUDE_DIR}/geometry.h
                               ${INCLUDE_DIR}/Viewer.h
                               ${INCLUDE_DIR}/Window.h
                               ${INCLUDE_DIR}/App.h
                               ${INCLUDE_DIR}/Framebuffer.h
                               ${INCLUDE_DIR}/Delaunay.h
                               )

target_link_libraries(${PROJECT_NAME} gkit
                                      imgui)
                                      
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

target_compile_definitions(${PROJECT_NAME} PUBLIC DATA_DIR="${DATA_DIR}"
                                                  OFF_DIR="${OFF_DIR}"
                                                  OBJ_DIR="${OBJ_DIR}"
                                                  SHADER_DIR="${SHADER_DIR}"
                                                  CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
                                                  )
