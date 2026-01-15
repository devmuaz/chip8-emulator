cmake_minimum_required(VERSION 3.20)
project(Chip8_Emulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib QUIET)

if(NOT raylib_FOUND)
    message(STATUS "raylib not found via find_package, using FetchContent")
    include(FetchContent)
    
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
    )

    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
    
    FetchContent_MakeAvailable(raylib)

    if(TARGET raylib_static)
        add_library(raylib ALIAS raylib_static)
    endif()
endif()

add_library(Chip8_Core
        src/Core/Chip8.cpp
        src/Core/Chip8.h
        src/Core/InstructionExecutor.cpp
        src/Core/InstructionExecutor.h
)

add_executable(Chip8_Emulator
        src/main.cpp
        src/App/Application.cpp
        src/App/Application.h
        src/App/Renderer.cpp
        src/App/Renderer.h
        src/App/Input.cpp
        src/App/Input.h
        src/App/Audio.cpp
        src/App/Audio.h
        src/App/ROMSelector.cpp
        src/App/ROMSelector.h
)

target_link_libraries(
        Chip8_Emulator
        PRIVATE Chip8_Core
        PRIVATE raylib
)

target_include_directories(Chip8_Core PUBLIC src/Core)
target_include_directories(Chip8_Emulator PRIVATE src/App)

if(WIN32)
    file(COPY roms DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY roms DESTINATION ${CMAKE_BINARY_DIR}/Release)
    file(COPY roms DESTINATION ${CMAKE_BINARY_DIR}/Debug)
else()
    file(COPY roms DESTINATION ${CMAKE_BINARY_DIR})
endif()

set_target_properties(Chip8_Emulator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)