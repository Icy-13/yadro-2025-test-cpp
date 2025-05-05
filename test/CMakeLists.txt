cmake_minimum_required(VERSION 3.27)
project(yadro_test_task)

file(GLOB TEST *.cpp *.h)
file(GLOB APP ../src/app/*.cpp ../src/app/*.h)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests ${TEST} ${APP})
include_directories(../src/app)

target_link_libraries(tests GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)