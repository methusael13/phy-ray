# CMake for PhyRayApp
# @author: Methusael Murmu

project(phyrapp VERSION 0.1 LANGUAGES CXX)

add_executable(phyrapp
    src/main.cpp
#    src/phyraywindow.cpp
#    forms/phyraywindow.ui
)

target_link_libraries(phyrapp PRIVATE phyrlib Qt5::Widgets)
# target_link_libraries(phyrapp PRIVATE phyrlib)
