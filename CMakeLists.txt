cmake_minimum_required(VERSION 3.16)
project(gam VERSION 0.0.1 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(NoInSourceBuilds)

include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Enable IPO (interprocedural optimization), if possible
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${ipo_supported})

include(Coverage)
include(Format)
include(CppCheck)
include(Doxygen)

# Building external libs 
add_subdirectory(vendor)

# Building sources
add_subdirectory(src bin)

Doxygen(gam src)

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


# For unit testing
include(Testing)
add_subdirectory(test)

