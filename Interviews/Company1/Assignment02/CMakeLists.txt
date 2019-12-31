cmake_minimum_required(VERSION 2.6)
project(assignment02)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3 -O0")


add_executable(assignment02 main.cpp SocialNetwork.cpp Test.cpp)

install(TARGETS assignment02 RUNTIME DESTINATION bin)
