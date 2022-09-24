cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(APPLE)
  set(SDL2_INCLUDE_DIR "/Library/Frameworks/SDL2.framework/Headers")
  set(SDL2_MIXER_INCLUDE "/Library/Frameworks/SDL2_mixer.framework/Headers")
  find_library(SDL2_LIBRARY SDL2 REQUIRED)
  find_library(SDL2_MIXER_LIBRARY SDL2_mixer REQUIRED)
  message(STATUS ${SDL2_MIXER_LIBRARY})
  include_directories(${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE})
  add_executable(example ChangeColorSquare.cpp)
  target_link_libraries(example ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARY})
endif(APPLE)

if(MINGW)
  set(SOURCES ChangeColorSquare.cpp)
  add_executable(example ${SOURCES})
  set(SDL2_DIR "C:/dev/SDL2-2.24.0/cmake")
  find_package(SDL2 REQUIRED)

  if(TARGET SDL2::SDL2main)
    target_link_libraries(example PRIVATE SDL2::SDL2main)
  endif()

  target_link_libraries(example PRIVATE SDL2::SDL2)
  file(COPY "C:/dev/SDL2-2.24.0/x86_64-w64-mingw32/bin/SDL2.dll" DESTINATION ${CMAKE_BINARY_DIR})
endif(MINGW)


