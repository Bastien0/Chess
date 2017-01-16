cmake_minimum_required(VERSION 2.6)
project(Echecs)
add_executable(Echecs
	       main.cpp
               Chess_Grid.h Chess_Grid.cpp
               Chessmen.h Chessmen.cpp
               point.h
               )
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
