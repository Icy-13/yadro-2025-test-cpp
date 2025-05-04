cmake_minimum_required(VERSION 3.27)
project(yadro_test_task)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCE src/*.cpp src/*.h)

add_executable(yadro_test_task ${SOURCE})
