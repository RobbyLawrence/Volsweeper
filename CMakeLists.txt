cmake_minimum_required(VERSION 3.10)
project(Volsweeper VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories
include_directories(include)

# Add executable
add_executable(volsweeper
    src/game.cpp
    src/volsweeper.cpp
    src/botV1.cpp
)
