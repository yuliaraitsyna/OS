set(TEST_SOURCES tests.cpp)

add_executable(runUnitTests ${TEST_SOURCES})
target_link_libraries(runUnitTests gtest gtest_main)
add_test(runUnitTests runUnitTests)
