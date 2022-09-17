cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(APPLE)
  set(SDL2_INCLUDE_DIR "/Library/Frameworks/SDL2.framework/Headers")
  find_library(SDL2_LIBRARY SDL2 REQUIRED)
  include_directories(${SDL2_INCLUDE_DIR})
  add_executable(example movingRedSquare.cpp)
  target_link_libraries(example ${SDL2_LIBRARY})
endif(APPLE)

if(MINGW)
  set(SOURCES movingRedSquare.cpp)
  set(SDL2_INCLUDE_DIR "C:/dev/SDL2-2.24.0/x86_64-w64-mingw32/include")
  set(SDL2_LIBRARY_DIR "C:/dev/SDL2-2.24.0/x86_64-w64-mingw32/lib")
  include_directories(${SDL2_INCLUDE_DIR})
  add_executable(example ${SOURCES})
  target_link_libraries(example -lmingw32 ${SDL2_LIBRARY_DIR}/libSDL2main.a
                        ${SDL2_LIBRARY_DIR}/libSDL2.dll.a -mwindows)
endif(MINGW)
