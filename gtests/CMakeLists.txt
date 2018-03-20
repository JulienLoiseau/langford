project (gtests)

add_subdirectory(googletest)


include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

add_executable(big_integer_test
        big_integers_test.cpp)
target_link_libraries(big_integer_test gtest gtest_main)
gtest_add_tests(
        TARGET      big_integer_test
        TEST_SUFFIX .noArgs
        TEST_LIST   noArgsTests)

add_executable(result_test
        result_test.cpp)
target_link_libraries(result_test gtest gtest_main)
gtest_add_tests(
        TARGET      result_test
        TEST_SUFFIX .noArgs
        TEST_LIST   noArgsTests)
