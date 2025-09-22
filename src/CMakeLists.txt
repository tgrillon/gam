set(SOURCE_DIR "Source")
set(INCLUDE_DIR "Include")
add_executable(${PROJECT_NAME} main.cpp
        ${SOURCE_DIR}/Utils.cpp
        ${SOURCE_DIR}/TMesh.cpp
        ${SOURCE_DIR}/Geometry.cpp
        ${SOURCE_DIR}/Viewer.cpp
        ${SOURCE_DIR}/Window.cpp
        ${SOURCE_DIR}/App.cpp
        ${SOURCE_DIR}/Framebuffer.cpp
        ${SOURCE_DIR}/Timer.cpp
        ${SOURCE_DIR}/pch.cpp

        ${INCLUDE_DIR}/Utils.h
        ${INCLUDE_DIR}/TMesh.h
        ${INCLUDE_DIR}/Geometry.h
        ${INCLUDE_DIR}/Viewer.h
        ${INCLUDE_DIR}/Window.h
        ${INCLUDE_DIR}/App.h
        ${INCLUDE_DIR}/Framebuffer.h
        ${INCLUDE_DIR}/Timer.h
        ${INCLUDE_DIR}/pch.h
)

target_link_libraries(gam PRIVATE gkit imgui)
target_include_directories(gam PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIR})
target_precompile_headers(gam PRIVATE ${INCLUDE_DIR}/pch.h)

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
        ${SOURCE_DIR}/pch.cpp
        # Don't include main.cpp in the static library
)

target_link_libraries(gam_static PUBLIC gkit imgui)
target_include_directories(gam_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIR})
target_precompile_headers(gam_static PRIVATE ${INCLUDE_DIR}/pch.h)
