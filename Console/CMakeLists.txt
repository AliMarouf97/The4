cmake_minimum_required(VERSION 3.0)

# set the project name
project(The4)

include_directories(${The4_SOURCE_DIR}/inc
../)
add_library(Game ../Game/Game.h inc/GameViewer.h ../Game/Game.cpp src/GameViewer.cpp)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(test_game src/test_game.cpp)
target_link_libraries(test_game PRIVATE Game)
