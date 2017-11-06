# CMake for PhyRayApp
# @author: Methusael Murmu

project(phyrapp VERSION 0.1 LANGUAGES CXX)

add_executable(phyrapp
    src/main.cpp
#    src/phyraywindow.cpp
#    forms/phyraywindow.ui
)

target_include_directories(phyrlib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE src
)

# target_link_libraries(phyrapp phyrlib Qt5::Widgets)
target_link_libraries(phyrapp phyrlib)
