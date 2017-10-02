# CMake for PhyRay
# @author: Methusael Murmu

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(PROJECT_NAME PhyRay)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

enable_testing()
add_subdirectory(phyray_lib)
add_subdirectory(phyray_app)
