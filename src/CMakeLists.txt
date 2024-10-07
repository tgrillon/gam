set(SOURCE_DIR "Source")
set(INCLUDE_DIR "Include")
add_executable(${PROJECT_NAME} main.cpp 
                               ${SOURCE_DIR}/TMesh.cpp
                               ${SOURCE_DIR}/geometry.cpp
                               ${SOURCE_DIR}/Viewer.cpp
                               ${SOURCE_DIR}/Window.cpp
                               ${SOURCE_DIR}/App.cpp
                               ${SOURCE_DIR}/Framebuffer.cpp
                               ${SOURCE_DIR}/Delaunay.cpp
                               ${SOURCE_DIR}/pch.cpp
                               ${INCLUDE_DIR}/utils.h
                               ${INCLUDE_DIR}/TMesh.h
                               ${INCLUDE_DIR}/geometry.h
                               ${INCLUDE_DIR}/Viewer.h
                               ${INCLUDE_DIR}/Window.h
                               ${INCLUDE_DIR}/App.h
                               ${INCLUDE_DIR}/Framebuffer.h
                               ${INCLUDE_DIR}/Delaunay.h
                               ${INCLUDE_DIR}/pch.h
                               )
 
target_link_libraries(${PROJECT_NAME} PRIVATE gkit
                                              imgui
                                              )
                                              
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR}) 
target_precompile_headers(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR/pch.h})

set(DATA_DIR "${CMAKE_SOURCE_DIR}/data" CACHE PATH "Path to the data directory")
message(STATUS "Data directory set to: ${DATA_DIR}")
set(OFF_DIR "${DATA_DIR}/off" CACHE PATH "Path to the off files")
message(STATUS "Off files directory set to: ${OFF_DIR}")
set(OBJ_DIR "${DATA_DIR}/obj" CACHE PATH "Path to the obj files")
message(STATUS "Obj files directory set to: ${OBJ_DIR}")
set(SHADER_DIR "${DATA_DIR}/shaders" CACHE PATH "Path to the shader files")
message(STATUS "Shader files directory set to: ${SHADER_DIR}")

target_compile_definitions(${PROJECT_NAME} PUBLIC DATA_DIR="${DATA_DIR}"
                                                  OFF_DIR="${OFF_DIR}"
                                                  OBJ_DIR="${OBJ_DIR}"
                                                  SHADER_DIR="${SHADER_DIR}"
                                                  CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
                                                  )   
                                           