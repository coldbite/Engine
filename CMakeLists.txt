cmake_minimum_required(VERSION 3.16)
project(Coldbite CXX)
project(Masterball VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Force MinGW g++ compiler
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)

# Engine Core Library
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "Engine/*.cpp"
    "Engine/*.h"
    #"Engine/*/.gitkeep"
)

file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    "Game/*.cpp"
    "Game/*.h"
    "Game/Game.conf"
   # "Game/*/.gitkeep"
)
# Create Engine library
add_library(Engine STATIC ${ENGINE_SOURCES})
target_include_directories(Engine PUBLIC Engine/Core)

# Depencies
set(FREETYPE_DIR "${CMAKE_SOURCE_DIR}/Depencies/FreeType")
target_include_directories(Engine PRIVATE ${FREETYPE_DIR}/include)

find_library(FREETYPE_LIBRARY
    NAMES freetype
    HINTS ${FREETYPE_DIR}/lib
)

if(FREETYPE_LIBRARY)
    target_link_libraries(Engine PRIVATE ${FREETYPE_LIBRARY})
else()
    message(FATAL_ERROR "[Error] Can't find FreeType in /Depencies/")
endif()

# Game executable
add_executable(Masterball ${GAME_SOURCES})
target_link_libraries(Masterball PRIVATE Engine)

# Always copy Game.conf to build directory
add_custom_target(copy_config ALL
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/Game/Game.conf"
    "${CMAKE_CURRENT_BINARY_DIR}/Game.conf"
    DEPENDS "${CMAKE_SOURCE_DIR}/Game/Game.conf"
    COMMENT "Copying Game.conf to build directory"
)

# Make Masterball depend on config copy
add_dependencies(Masterball copy_config)

# Enable threading support
find_package(Threads REQUIRED)
target_link_libraries(Engine PRIVATE Threads::Threads)

# OpenGL support for NativeWindow
find_package(OpenGL REQUIRED)
target_link_libraries(Engine PRIVATE ${OPENGL_LIBRARIES})

# Windows-specific libraries for window management
if(WIN32)
    target_link_libraries(Engine PRIVATE gdi32 user32 kernel32)
endif()

# Compiler specific settings
target_compile_options(Engine PRIVATE -Wall -Wextra -pedantic)
target_compile_options(Masterball PRIVATE -Wall -Wextra -pedantic)

# Debug/Release settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Engine PRIVATE DEBUG)
    target_compile_definitions(Masterball PRIVATE DEBUG)
endif()

message(STATUS "Configured Event-based Multithreaded Engine")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
