# CMake for PhyRay Library
# @author: Methusael Murmu

project(lib VERSION 0.1 LANGUAGES CXX)

add_library(lib
    src/core/geometry/geometry.cpp
    src/core/geometry/transform.cpp
)

target_include_directories(lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src
)

target_link_libraries(lib m pthread)

add_executable(phyr_test
    test/testlib.cpp
)

target_link_libraries(phyr_test lib)
add_test(phyr_test phyr_test)
