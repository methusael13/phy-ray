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

set(COMMON_LIBS lib m pthread)
target_link_libraries(${COMMON_LIBS})

set(TEST_EXE
    test_vec test_fpe test_math
)
foreach(test_exe ${TEST_EXE})
    add_executable(${test_exe} test/${test_exe}.cpp)
    target_link_libraries(${test_exe} ${COMMON_LIBS})
    add_test(${test_exe} ${test_exe})
endforeach(test_exe)
