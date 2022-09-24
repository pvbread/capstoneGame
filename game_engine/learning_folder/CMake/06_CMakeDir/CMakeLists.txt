cmake_minimum_required(VERSION 3.10)

project(PhoenixEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

if(APPLE)
  set(SDL2_INCLUDE_DIR "/Library/Frameworks/SDL2.framework/Headers")
  find_library(SDL2_LIBRARY SDL2 REQUIRED)
  include_directories(${SDL2_INCLUDE_DIR})
  add_executable(helloGame
    "src/gameLoop.cpp"
    "src/gameEngine.cpp"
    "include/gameEngine.h"
  )
  target_link_libraries(helloGame ${SDL2_LIBRARY})
endif(APPLE)