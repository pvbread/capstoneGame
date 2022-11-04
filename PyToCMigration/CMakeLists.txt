cmake_minimum_required(VERSION 3.10)

#set the project name, might have to fix version
project(myProject VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(testingPython 
    "src/test.cpp"
    "src/Characters/BaseCharacter.cpp"
)