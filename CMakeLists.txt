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
)

file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    "Game/*.cpp"
    "Game/*.h"
)

# Create Engine library
add_library(Engine STATIC ${ENGINE_SOURCES})
target_include_directories(Engine PUBLIC Engine/Core)

# Game executable
add_executable(Masterball ${GAME_SOURCES})
target_link_libraries(Masterball Engine)

# Copy Game.conf to build directory
add_custom_command(TARGET Masterball POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/Game/Game.conf"
    "${CMAKE_CURRENT_BINARY_DIR}/Game.conf"
    COMMENT "Copying Game.conf to build directory"
)

# Enable threading support
find_package(Threads REQUIRED)
target_link_libraries(Engine Threads::Threads)

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
