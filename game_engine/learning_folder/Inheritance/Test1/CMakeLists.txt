cmake_minimum_required(VERSION 3.10)

# set the project name
project(Test VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(inheritance main.cpp)