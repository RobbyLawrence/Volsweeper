#building python extension

cmake_minimum_required(VERSION 3.14)
project(VolSweeperPy LANGUAGES CXX)

# 1) Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)  # for shared libs

# 2) Locate PyBind11 (installed via pybind11-dev)
find_package(pybind11 REQUIRED)

# 3) Module name “volsweeper_binding”
pybind11_add_module(volsweeper_binding
    bindings.cpp
    # point to your core logic .cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/volsweeper.cpp
)

# 4) Tell it where to find your headers
target_include_directories(volsweeper_binding
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

