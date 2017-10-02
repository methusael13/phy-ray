# CMake for PhyRay
# @author: Methusael Murmu

set(PROJECT_NAME PhyRay)

cmake_minimum_required(VERSION 2.8)
project(${PROJECT_NAME})

set(PROJECT_SRC src)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LOCAL_LIB_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# Enable C++11 standard and level 2 optimization
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
INCLUDE_DIRECTORIES(include)

set(COMMON_LIBS m pthread)
set(ENTRY_POINT main)

add_executable(${ENTRY_POINT} ${PROJECT_SRC}/app/${ENTRY_POINT}.cpp)
target_link_libraries(${ENTRY_POINT} ${COMMON_LIBS})
