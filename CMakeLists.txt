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

include(ConfigPresets)

include(Coverage)
include(Format)
include(CppCheck)
include(Doxygen)

# Building external libs 
add_subdirectory(vendor)

# Building sources
add_subdirectory(src bin)

Doxygen(gam src)

include(SetupEnvVariables)

# For unit testing
include(Testing) 
add_subdirectory(test)

