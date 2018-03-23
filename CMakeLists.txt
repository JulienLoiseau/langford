cmake_minimum_required(VERSION 3.9)
project(langford_godfrey)
include(GoogleTest)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

if(ENABLE_COVERAGE EQUAL ON)
    include(CodeCoverage)
    add_definitions( -fprofile-arcs -ftest-coverage )
    target_link_libraries(langford_godfrey gcov)
endif()

enable_testing()
include_directories(src)

# Tests
add_subdirectory(gtests)

# Main executable
add_executable(langford_godfrey
        src/main.cpp)
