cmake_minimum_required(VERSION 3.16)
project(Coldbite CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force MinGW g++ compiler
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)

# Engine Core Library
set(ENGINE_SOURCES
    Engine/Core/Engine.cpp
    Engine/Core/ThreadPool.cpp
    Engine/Core/RenderManager.cpp
    Engine/Core/Game.cpp
    Engine/Core/CL/Parser.cpp
    Engine/Core/Settings/Options.cpp
    Engine/Core/Settings/Config.cpp
)

set(ENGINE_HEADERS
    Engine/Core/Engine.h
    Engine/Core/Event.h
    Engine/Core/ThreadPool.h
    Engine/Core/RenderManager.h
    Engine/Core/Game.h
    Engine/Core/CL/Parser.h
    Engine/Core/Settings/Options.h
    Engine/Core/Settings/Config.h
)

set(GAME_SOURCES
    Game/main.cpp
    Game/Masterball.cpp
)

set(GAME_HEADERS
    Game/Masterball.h
    Game/Options.h
)

# Create Engine library
add_library(Engine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})
target_include_directories(Engine PUBLIC Engine/Core)

# Game executable
add_executable(Game ${GAME_SOURCES} ${GAME_HEADERS})
target_link_libraries(Game Engine)

# Copy Game.conf to build directory
add_custom_command(TARGET Game POST_BUILD
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
target_compile_options(Game PRIVATE -Wall -Wextra -pedantic)

# Debug/Release settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Engine PRIVATE DEBUG)
    target_compile_definitions(Game PRIVATE DEBUG)
endif()

message(STATUS "Configured Event-based Multithreaded Engine")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
