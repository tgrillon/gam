set(SOURCE_DIR "Source")
set(INCLUDE_DIR "Include")
add_executable(${PROJECT_NAME} main.cpp
        ${SOURCE_DIR}/Utils.cpp
        ${SOURCE_DIR}/TMesh.cpp
        ${SOURCE_DIR}/Geometry.cpp
        ${SOURCE_DIR}/Viewer.cpp
        # ${SOURCE_DIR}/Window.cpp
        ${SOURCE_DIR}/App.cpp
        ${SOURCE_DIR}/Framebuffer.cpp
        ${SOURCE_DIR}/Timer.cpp
)

target_link_libraries(gam PRIVATE gkit imgui warning_properties)
target_include_directories(gam PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIR})
target_precompile_headers(gam PRIVATE ${INCLUDE_DIR}/PCH/PCH.h)

# Create a static library for testing
add_library(gam_static STATIC
        ${SOURCE_DIR}/Utils.cpp
        ${SOURCE_DIR}/TMesh.cpp
        ${SOURCE_DIR}/Geometry.cpp
        ${SOURCE_DIR}/Viewer.cpp
        ${SOURCE_DIR}/Window.cpp
        ${SOURCE_DIR}/App.cpp
        ${SOURCE_DIR}/Framebuffer.cpp
        ${SOURCE_DIR}/Timer.cpp
)

target_link_libraries(gam_static PUBLIC gkit imgui warning_properties)
target_include_directories(gam_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIR})
target_precompile_headers(gam_static PUBLIC ${INCLUDE_DIR}/PCH/PCH.h)

# Declare warning flags interface 
add_library(warning_properties INTERFACE)
target_compile_options(warning_properties INTERFACE -Wall -Wextra -Wpedantic)
