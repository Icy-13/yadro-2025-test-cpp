cmake_minimum_required(VERSION 3.27)
project(yadro_test_task)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

file(GLOB APP src/app/*.cpp src/app/*.h)

add_executable(task ${APP} src/main.cpp)
add_subdirectory(test)