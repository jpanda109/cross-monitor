cmake_minimum_required(VERSION 3.1)
project(linux_events)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

set(SOURCE_FILES mouse.c)
add_executable(linux_events ${SOURCE_FILES})
target_link_libraries(linux_events X11 pthread)