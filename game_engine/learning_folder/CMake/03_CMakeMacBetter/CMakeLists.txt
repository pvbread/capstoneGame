cmake_minimum_required(VERSION 3.10)

#set the project name, might have to fix version
project(Tutorial VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SDL2_INCLUDE_DIR "/Library/Frameworks/SDL2.framework/Headers")
find_library(SDL2_LIBRARY SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
add_executable(example movingRedSquare.cpp)
target_link_libraries(example ${SDL2_LIBRARY})
