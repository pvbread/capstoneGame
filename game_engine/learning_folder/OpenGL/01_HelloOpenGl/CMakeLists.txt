cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

if(APPLE)
  set(SDL2_INCLUDE_DIR "/Library/Frameworks/SDL2.framework/Headers")
  set(SDL2_MIXER_INCLUDE "/Library/Frameworks/SDL2_mixer.framework/Headers")
  find_library(SDL2_LIBRARY SDL2 REQUIRED)
  find_library(SDL2_MIXER_LIBRARY SDL2_mixer REQUIRED)
  include_directories(${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE})
  add_executable(helloOpenGl
    "helloOpenGl.cpp"
    "src/glad.c"
    "include/glad/glad.h"
  )
  target_link_libraries(helloOpenGl ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARY})
endif(APPLE)

if(MINGW)
  add_executable(helloOpenGl
    "helloOpenGl.cpp"
    "src/glad.c"
    "include/glad/glad.h"
  )
  set(SDL2_DIR "C:/dev/SDL2-2.24.0/cmake")
  set(SDL2_mixer_DIR "C:/dev/SDL2_mixer-2.6.2/cmake")
  find_package(SDL2 REQUIRED)
  find_package(SDL2_mixer REQUIRED)

  if(TARGET SDL2::SDL2main)
    target_link_libraries(helloOpenGl PRIVATE SDL2::SDL2main)
  endif()

  target_link_libraries(helloOpenGl PRIVATE SDL2::SDL2 SDL2_mixer::SDL2_mixer)
endif(MINGW)

