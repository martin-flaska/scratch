cmake_minimum_required(VERSION 3.0)

project(platformdispatcher)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(
 ./include
)

add_executable(platformdispatcher dispatcher_challenge.cpp)

install(TARGETS platformdispatcher RUNTIME DESTINATION bin)
