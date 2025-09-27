# Définir les types de build disponibles
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel" CACHE STRING "Available build configurations" FORCE)

# Définir Debug comme configuration par défaut si non spécifiée
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Configuration Debug
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -DDEBUG")

# Configuration Release  
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Configuration RelWithDebInfo
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")

# Configuration MinSizeRel
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")

# Afficher la configuration actuelle
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")

# Configuration spécifique selon le type de build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build - Adding debug-specific settings")
    # Ajouter des bibliothèques de debug, options spécifiques, etc.
    add_compile_definitions(ENABLE_LOGGING=1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release build - Adding release-specific settings")
    # Optimisations spécifiques au release
    add_compile_definitions(ENABLE_LOGGING=0)
endif()
