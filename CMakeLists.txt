cmake_minimum_required(VERSION 3.9)
include(GoogleTest)
project(langford_godfrey)

if(COVERAGE)
    add_definitions( -fprofile-arcs -ftest-coverage )
endif(COVERAGE)

enable_testing()

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

# Tests
add_subdirectory(gtests)

# Main executable
add_executable(langford_godfrey
        src/big_integers.hpp
        src/main.cpp src/gray.hpp src/task.hpp src/instance.hpp)
