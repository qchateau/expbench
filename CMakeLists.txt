cmake_minimum_required(VERSION 3.14)
project(expbench CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

include(FetchContent)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.4
)

FetchContent_MakeAvailable(benchmark)

add_executable(
    expbench
    expbench.cpp
)
target_link_libraries(
    expbench
    PRIVATE
        benchmark::benchmark
        benchmark::benchmark_main
)
target_compile_options(
    expbench
    PRIVATE -march=native
)


add_executable(
    csv
    csv.cpp
)
