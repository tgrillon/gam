cmake_minimum_required (VERSION 3.16)

project(gam)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Fetching gkit library 
add_subdirectory(vendor/gkit)
# Fetching imgui library 
add_subdirectory(vendor/imgui)

add_subdirectory(src)                    
