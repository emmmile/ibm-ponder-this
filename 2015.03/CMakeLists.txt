project(ternary)
cmake_minimum_required(VERSION 2.8)
add_definitions(-std=c++11)
set(CMAKE_VERBOSE_MAKEFILE false)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g3 -mtune=native -ffast-math -funroll-loops" )
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS}")

include_directories ("${PROJECT_SOURCE_DIR}")

add_executable(tape tape.cpp)

target_link_libraries(tape boost_program_options boost_log boost_system)
