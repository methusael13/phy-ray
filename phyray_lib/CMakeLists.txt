# CMake for PhyRay Library
# @author: Methusael Murmu

project(phyrlib VERSION 0.1 LANGUAGES CXX)

add_library(phyrlib STATIC
    # Core library
    src/core/phyr.cpp
    src/core/phyr_mem.cpp
    src/core/phyr_math.cpp
    src/core/geometry/geometry.cpp
    src/core/geometry/transform.cpp
    src/core/geometry/interaction.cpp
    src/core/geometry/shape.cpp
    src/core/object/object.cpp
    src/core/color/spectrum.cpp
    src/core/film.cpp
    src/core/concurrency.cpp
    src/core/debug.cpp
    src/core/imageio.cpp
    src/core/scene.cpp
    src/core/lowdiscrepancy.cpp

    # Lights
    src/core/light/light.cpp
    src/modules/lights/diffuse.cpp
    src/modules/lights/distant.cpp
    src/modules/lights/point.cpp

    # Materials
    src/core/material/material.cpp
    src/core/material/reflectance.cpp
    src/core/material/microfacet.cpp
    src/core/material/texture.cpp
    
    # Supported materials
    src/modules/materials/matte.cpp
    src/modules/materials/plastic.cpp
    src/modules/materials/metal.cpp
    src/modules/materials/glass.cpp
    src/modules/materials/mirror.cpp
    
    # Textures
    src/modules/textures/consttex.cpp
    src/modules/textures/scaled.cpp
    src/modules/textures/mix.cpp

    # Cameras
    src/core/camera/camera.cpp
    src/modules/cameras/perspective.cpp
    
    # Accelerators
    src/core/accel/bvh.cpp

    # Shapes
    src/modules/shapes/sphere.cpp
    
    # Integrator and samplers
    src/core/integrator/integrator.cpp
    src/core/integrator/sampler.cpp
    src/core/integrator/sampling.cpp
    src/core/integrator/filter.cpp
    src/core/integrator/lightdistrib.cpp
    src/modules/samplers/stratified.cpp
    src/modules/integrators/path.cpp
    
    # Filters
    src/modules/filters/box.cpp
    src/modules/filters/triangle.cpp
    src/modules/filters/gaussian.cpp
    src/modules/filters/mitchell.cpp
    src/modules/filters/lanczos.cpp
)

target_include_directories(phyrlib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${ILMBASE_INCLUDE_DIRS}
    ${OPENEXR_INCLUDE_DIRS}
    PRIVATE src
)

set(OPENEXR_DEP_LIBS ${OPENEXR_LIBRARIES} ${ILMBASE_LIBRARIES})
set(PHYRAY_DEP_LIBS ${OPENEXR_DEP_LIBS} m pthread)
target_link_libraries(phyrlib ${PHYRAY_DEP_LIBS})

set(PHYRAY_LIBS phyrlib ${OPENEXR_DEP_LIBS} m pthread)

# Make the project importable from the build directory
# export(TARGETS phyrlib FILE PhyrayLibConfig.cmake)

# Define test targets
set(TEST_EXE
    test_vec test_fpe test_math
    test_isec test_mem test_consttex
    test_point
)
foreach(test_exe ${TEST_EXE})
    add_executable(${test_exe} test/${test_exe}.cpp)
    target_link_libraries(${test_exe} ${PHYRAY_LIBS})
    add_test(${test_exe} ${test_exe})
endforeach(test_exe)
