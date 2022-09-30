cmake_minimum_required(VERSION 3.10)

project(PhoenixEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

if(APPLE)
  set(SDL2_INCLUDE_DIR "/Library/Frameworks/SDL2.framework/Headers")
  set(SDL2_IMAGE_INCLUDE "/Library/Frameworks/SDL2_image.framework/Headers")
  find_library(SDL2_LIBRARY SDL2 REQUIRED)
  find_library(SDL2_IMAGE_LIBRARY SDL2_image REQUIRED)
  include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE})
  add_executable(example
    "src/gameLoop.cpp"
    "src/gameEngine.cpp"
    "include/gameEngine.h"
  )
  target_link_libraries(example ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})
endif(APPLE)

if(MINGW)
  add_executable(example
    "src/gameLoop.cpp"
    "src/gameEngine.cpp"
    "include/gameEngine.h"
  )
  set(SDL2_DIR "C:/dev/SDL2-2.24.0/cmake")
  set(SDL2_image_DIR "C:/dev/SDL2_image-2.6.2/cmake")
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)

  if(TARGET SDL2::SDL2main)
    target_link_libraries(example PRIVATE SDL2::SDL2main)
  endif()

  target_link_libraries(example PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)
endif(MINGW)