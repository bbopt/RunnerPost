# tests/CMakeLists.txt
add_executable(test_algorithm test_algorithm.cpp)
target_link_libraries(test_algorithm PRIVATE gtest gtest_main runnerPost)
add_test(NAME test_algorithm COMMAND test_algorithm)


add_executable(test_output test_output.cpp)
target_link_libraries(test_output PRIVATE gtest gtest_main runnerPost)
add_test(NAME test_output COMMAND test_output)

add_executable(test_problem test_problem.cpp)
target_link_libraries(test_problem PRIVATE gtest gtest_main runnerPost)
add_test(NAME test_problem COMMAND test_problem)

add_executable(test_runner test_runner.cpp)
target_link_libraries(test_runner PRIVATE gtest gtest_main runnerPost)
add_test(NAME test_runner COMMAND test_runner)
