cmake_minimum_required(VERSION 2.6)
project(Echecs)
add_executable(Echecs
               Chess_Grid.h Chess_Grid.cpp
               Chessmen.h Chessmen.cpp
               )
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
