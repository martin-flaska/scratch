cmake_minimum_required(VERSION 3.0)

project(lockguard)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(lockguard main.cpp)

find_package (Threads)

target_link_libraries(lockguard
	${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS lockguard RUNTIME DESTINATION bin)
