cmake_minimum_required(VERSION 3.14)
project(gtest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(
  SOURCES
  ../dag.c
  test.cpp
)

set(
  HEADERS
  ../dag.h
)

enable_testing()

add_executable(dag-test ${SOURCES})

target_include_directories(dag-test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ../)

target_link_libraries(dag-test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(dag-test)
