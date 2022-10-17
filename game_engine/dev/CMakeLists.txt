cmake_minimum_required(VERSION 3.10)

project(PhoenixEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

if(APPLE)
  set(SDL2_INCLUDE_DIR "/Library/Frameworks/SDL2.framework/Headers")
  set(SDL2_IMAGE_INCLUDE "/Library/Frameworks/SDL2_image.framework/Headers")
  set(SDL2_TTF_INCLUDE "/Library/Frameworks/SDL2_ttf.framework/Headers")
  set(SDL2_MIXER_INCLUDE "/Library/Frameworks/SDL2_mixer.framework/Headers")
  find_library(SDL2_LIBRARY SDL2 REQUIRED)
  find_library(SDL2_IMAGE_LIBRARY SDL2_image REQUIRED)
  find_library(SDL2_TTF_LIBRARY SDL2_ttf REQUIRED)
  find_library(SDL2_MIXER_LIBRARY SDL2_mixer REQUIRED)
  include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE} ${SDL2_TTF_INCLUDE} ${SDL2_MIXER_INCLUDE})
  add_executable(capstoneGame
    "src/gameLoop.cpp"
    "src/gameEngine.cpp"
    "src/TextureWrapper.cpp"
    "src/Tile.cpp"
    "src/MapDebugController.cpp"
    "src/MenuInGame.cpp"
  )
  target_link_libraries(capstoneGame ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_MIXER_LIBRARY})
  file(COPY tilesDraft.png DESTINATION ${CMAKE_BINARY_DIR})
  file(COPY dot.bmp DESTINATION ${CMAKE_BINARY_DIR})
  file(COPY testLevel.map DESTINATION ${CMAKE_BINARY_DIR})
  file(COPY Raleway-Medium.ttf DESTINATION ${CMAKE_BINARY_DIR})
  file(COPY bgmusic1.wav DESTINATION ${CMAKE_BINARY_DIR})
  file(COPY menuMove.wav DESTINATION ${CMAKE_BINARY_DIR})
  file(COPY menuSelect.wav DESTINATION ${CMAKE_BINARY_DIR})
endif(APPLE)

if(MINGW)
  add_executable(capstoneGame
    "src/gameLoop.cpp"
    "src/gameEngine.cpp"
    "src/TextureWrapper.cpp"
    "src/Tile.cpp"
    "src/MapDebugController.cpp"
    "src/MenuInGame.cpp"
  )
  set(SDL2_DIR "C:/dev/SDL2-2.24.0/cmake")
  set(SDL2_image_DIR "C:/dev/SDL2_image-2.6.2/cmake")
  set(SDL2_ttf_DIR "C:/dev/SDL2_ttf/cmake") #fix this alex to whatever folder it is
  set(SDL2_mixer_DIR "C:/dev/SDL2_mixer/cmake")
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  find_package(SDL2_ttf REQUIRED)
  find_package(SDL2_mixer REQUIRED)

  if(TARGET SDL2::SDL2main)
    target_link_libraries(capstoneGame PRIVATE SDL2::SDL2main)
  endif()

  target_link_libraries(capstoneGame PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)
  file(COPY tilesDraft.png DESTINATION ${CMAKE_BINARY_DIR})
  file(COPY dot.bmp DESTINATION ${CMAKE_BINARY_DIR})
  file(COPY testLevel.map DESTINATION ${CMAKE_BINARY_DIR})
  
endif(MINGW)