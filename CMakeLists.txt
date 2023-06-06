cmake_minimum_required(VERSION 3.25)
project(RubikCube)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})


add_executable(RubikCube main.cpp Cube.cpp Square.cpp)

target_link_libraries(RubikCube ${SDL2_LIBRARIES})