# Data directories setup
set(DATA_DIR "${CMAKE_SOURCE_DIR}/data" CACHE PATH "Path to the data directory")
message(STATUS "Data directory set to: ${DATA_DIR}")
set(OFF_DIR "${DATA_DIR}/off" CACHE PATH "Path to the off files")
message(STATUS "Off files directory set to: ${OFF_DIR}")
set(OBJ_DIR "${DATA_DIR}/obj" CACHE PATH "Path to the obj files")
message(STATUS "Obj files directory set to: ${OBJ_DIR}")
set(SHADER_DIR "${DATA_DIR}/shaders" CACHE PATH "Path to the shader files")
message(STATUS "Shader files directory set to: ${SHADER_DIR}")
set(CLOUD_DIR "${DATA_DIR}/cloud" CACHE PATH "Path to the point cloud")
message(STATUS "Point cloud files directory set to: ${CLOUD_DIR}")

# Apply compile definitions to both main executable and static library
foreach (target gam gam_static)
    target_compile_definitions(${target} PUBLIC
            DATA_DIR="${DATA_DIR}"
            OFF_DIR="${OFF_DIR}"
            OBJ_DIR="${OBJ_DIR}"
            SHADER_DIR="${SHADER_DIR}"
            CLOUD_DIR="${CLOUD_DIR}"
            CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
    )
endforeach ()
