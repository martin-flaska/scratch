cmake_minimum_required(VERSION 2.6)
project(assignment01)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3 -O0")


add_executable(assignment01 main.cpp Storyboard.cpp Test.cpp)

install(TARGETS assignment01 RUNTIME DESTINATION bin)
