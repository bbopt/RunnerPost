# tests/CMakeLists.txt
add_executable(test_algorithm test_algorithm.cpp)
target_link_libraries(test_algorithm gtest gtest_main)
add_test(NAME test_algorithm COMMAND test_algorithm)