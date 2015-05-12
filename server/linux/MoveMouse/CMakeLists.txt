cmake_minimum_required(VERSION 3.1)
project(MoveMouse)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES main.cpp)
add_executable(MoveMouse ${SOURCE_FILES})
target_link_libraries(MoveMouse X11)
