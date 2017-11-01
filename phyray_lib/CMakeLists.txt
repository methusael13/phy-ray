# CMake for PhyRay Library
# @author: Methusael Murmu

project(lib VERSION 0.1 LANGUAGES CXX)

add_library(lib
    # Core library
    src/core/phyr.cpp
    src/core/phyr_mem.cpp
    src/core/phyr_math.cpp
    src/core/geometry/geometry.cpp
    src/core/geometry/transform.cpp
    src/core/geometry/interaction.cpp
    src/core/geometry/shape.cpp
    src/core/object/object.cpp
    src/core/material/material.cpp
    src/core/color/spectrum.cpp
    src/core/film.cpp
    src/core/concurrency.cpp
    src/core/debug.cpp
    
    # Cameras
    src/core/camera/camera.cpp
    src/modules/cameras/perspective.cpp
    
    # Accelerators
    src/core/accel/bvh.cpp

    # Shapes
    src/modules/shapes/sphere.cpp
    
    # Integrator and samplers
    src/core/integrator/sampler.cpp
    src/core/integrator/sampling.cpp
    src/core/integrator/filter.cpp
    src/modules/samplers/stratified.cpp
    
    # Filters
    src/modules/filters/box.cpp
    src/modules/filters/triangle.cpp
    src/modules/filters/gaussian.cpp
    src/modules/filters/mitchell.cpp
    src/modules/filters/lanczos.cpp
)

target_include_directories(lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src
)

set(COMMON_LIBS lib m pthread)
target_link_libraries(${COMMON_LIBS})

# Define test targets
set(TEST_EXE
    test_vec test_fpe test_math
    test_isec test_mem
)
foreach(test_exe ${TEST_EXE})
    add_executable(${test_exe} test/${test_exe}.cpp)
    target_link_libraries(${test_exe} ${COMMON_LIBS})
    add_test(${test_exe} ${test_exe})
endforeach(test_exe)
