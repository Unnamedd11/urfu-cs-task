cmake_minimum_required(VERSION 3.16)
project(callback_scheduler_task LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# Для MSVC, чтобы не переопределять runtime flags проекта.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(callback_scheduler
    src/callback_scheduler.cpp
)

target_include_directories(callback_scheduler
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()

add_executable(callback_scheduler_tests
    tests/callback_scheduler_test.cpp
)

target_link_libraries(callback_scheduler_tests
    PRIVATE
        callback_scheduler
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(callback_scheduler_tests)
