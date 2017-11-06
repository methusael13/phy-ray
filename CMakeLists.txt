# CMake for PhyRay
# @author: Methusael Murmu

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(PROJECT_NAME PhyRay)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Suppress deprecated warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")

# Add OpenEXR
find_package(PkgConfig REQUIRED)
pkg_search_module(OPENEXR REQUIRED OpenEXR)
pkg_search_module(ILMBASE REQUIRED IlmBase)

# Add Qt
# find_package(Qt5Core)
# find_package(Qt5Widgets)

# PhyRay
enable_testing()
add_subdirectory(phyray_lib)
add_subdirectory(phyray_app)
