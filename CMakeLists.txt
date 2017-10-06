# CMake for PhyRay
# @author: Methusael Murmu

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(PROJECT_NAME PhyRay)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()
add_subdirectory(phyray_lib)
add_subdirectory(phyray_app)
