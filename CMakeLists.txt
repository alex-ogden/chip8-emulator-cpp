
cmake_minimum_required(VERSION 3.16)
project(chip8-emulator)

# Use modern C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found. Please install it via Homebrew: brew install sdl2")
endif()

# Include directories for SDL2
include_directories(${SDL2_INCLUDE_DIRS})

# Add executable
add_executable(chip8-emulator src/main.cpp src/chip8.cpp src/memory.cpp src/cpu.cpp src/display.cpp src/input.cpp)

# Link SDL2 libraries
target_link_libraries(chip8-emulator ${SDL2_LIBRARIES})
