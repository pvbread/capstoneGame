cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Pita you Will need to change the proper include path for line 13 (I guessed)
if(APPLE)
  set(SDL2_INCLUDE_DIR "/Library/Frameworks/SDL2.framework/Headers")
  set(SDL2_TTF_INCLUDE "/Library/Frameworks/SDL2_ttf.framework/Headers")
  find_library(SDL2_LIBRARY SDL2 REQUIRED)
  find_library(SDL2_TTF_LIBRARY SDL2_ttf REQUIRED)
  message(STATUS "hello")
  message(STATUS ${SDL2_INCLUDE_LIBRARY})
  include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE})
  add_executable(example SDL_TimerTest.cpp)
  target_link_libraries(example ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})
endif(APPLE)

if(MINGW)
  set(SOURCES TimerTest.cpp)
  add_executable(example ${SOURCES})
  set(SDL2_DIR "C:/dev/SDL2-2.24.0/cmake")
  set(SDL2_ttf_DIR "C:/dev/SDL2_ttf-2.20.1/cmake")
  find_package(SDL2 REQUIRED)
  find_package(SDL2_ttf REQUIRED)

  if(TARGET SDL2::SDL2main)
    target_link_libraries(example PRIVATE SDL2::SDL2main)
  endif()

  target_link_libraries(example PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf)
endif(MINGW)
