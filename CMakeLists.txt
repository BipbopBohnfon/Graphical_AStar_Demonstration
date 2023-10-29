cmake_minimum_required(VERSION 3.25)
project(AStarProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(AStarProject main.cpp astar.h astar.cpp)
target_link_libraries(AStarProject sfml-graphics sfml-audio sfml-window sfml-system)