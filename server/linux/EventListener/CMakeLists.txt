cmake_minimum_required(VERSION 3.1)
project(linux_events)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES main.cpp)
add_executable(linux_events ${SOURCE_FILES})
target_link_libraries(linux_events X11 pthread)
