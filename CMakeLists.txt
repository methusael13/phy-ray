# CMake for PhyRay
# @author: Methusael Murmu

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(PROJECT_NAME PhyRay)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add OpenEXR
SET(ILMBASE_NAMESPACE_VERSIONING OFF CACHE BOOL " " FORCE)
SET(OPENEXR_NAMESPACE_VERSIONING OFF CACHE BOOL " " FORCE)
SET(OPENEXR_BUILD_SHARED_LIBS    OFF CACHE BOOL " " FORCE)
SET(ILMBASE_BUILD_SHARED_LIBS    OFF CACHE BOOL " " FORCE)

ADD_SUBDIRECTORY(external/openexr)

SET_PROPERTY(TARGET IexMath eLut toFloat b44ExpLogTable dwaLookups CopyIlmBaseLibs IlmThread Half Iex Imath IlmImf PROPERTY FOLDER "dependencies")

INCLUDE_DIRECTORIES(
  external/openexr/IlmBase/Imath
  external/openexr/IlmBase/Half
  external/openexr/IlmBase/Iex
  external/openexr/OpenEXR/IlmImf
  ${CMAKE_BINARY_DIR}/external/openexr/IlmBase/config
  ${CMAKE_BINARY_DIR}/external/openexr/OpenEXR/config
)
SET(OPENEXR_LIBS IlmImf Imath Half)

# PhyRay
enable_testing()
add_subdirectory(phyray_lib)
add_subdirectory(phyray_app)
