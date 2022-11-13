cmake_minimum_required(VERSION 3.10)

#set the project name, might have to fix version
project(myProject VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_FILES
    src/test.cpp
    src/test.cpp
    src/Characters/BaseCharacter.cpp
    src/Characters/Players/BasePlayer.cpp
    src/Characters/Players/Bass.cpp
)

set(LIBRARY_NAME pythonMigration)


include_directories(include)

add_subdirectory(test)
add_subdirectory(include)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

#add_library(Migration SHARED ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} ${LIBRARY_NAME})