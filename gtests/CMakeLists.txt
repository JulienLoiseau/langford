project (gtests)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(googletest-master)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

add_executable(big_integer_test
        big_integers_test.cpp)

add_executable(result_test
        result_test.cpp)

target_link_libraries(big_integer_test gtest gtest_main)
target_link_libraries(result_test gtest gtest_main)