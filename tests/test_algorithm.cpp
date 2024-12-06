
// tests/test_algorithm.cpp
#include <gtest/gtest.h>
#include "../src/Algorithm.hpp"

using namespace RUNNERPOST;

TEST(AlgorithmTest, Empty) {
    std::string error_msg;

    // When the algo constructor is called with an empty string, the id and name should be empty
    Algorithm algo("", error_msg);
    EXPECT_EQ(algo.get_id(), "");
    EXPECT_EQ(algo.get_name(), "");
    EXPECT_TRUE(algo.get_output_options().empty());
}

TEST(AlgorithmTest, Constructor) {
    std::string s = "Algo_1 (DAMON X: Default) [STATS_FILE_NAME history.txt][STATS_FILE_OUTPUT SOL OBJ CST ]  [ADD_PBINSTANCE_TO_STATS_FILE yes]";
    std::string error_msg;
    Algorithm algo(s, error_msg);
    EXPECT_EQ(algo.get_id(), "Algo_1");
    EXPECT_EQ(algo.get_name(), "DAMON X: Default");
    EXPECT_FALSE(algo.get_output_options().empty());

    std::vector<std::string> output_options = algo.get_output_options();
    EXPECT_EQ(output_options.size(), 3);
    EXPECT_EQ(output_options[0], "STATS_FILE_NAME history.txt");
    EXPECT_EQ(output_options[1], "STATS_FILE_OUTPUT SOL OBJ CST");
    EXPECT_EQ(output_options[2], "ADD_PBINSTANCE_TO_STATS_FILE yes");

    algo.resetInfo();
    EXPECT_EQ(algo.get_id(), "");
    EXPECT_EQ(algo.get_name(), "");
    EXPECT_TRUE(algo.get_output_options().empty());

}

TEST(AlgorithmTest, HandleComments) {
    std::string s = "Algo_1 (DAMON X: Default) [STATS_FILE_NAME history.txt] # This is a comment";
    std::string error_msg;
    Algorithm algo(s, error_msg);
    EXPECT_EQ(algo.get_id(), "Algo_1");
    EXPECT_EQ(algo.get_name(), "DAMON X: Default");
    EXPECT_FALSE(algo.get_output_options().empty());

    std::vector<std::string> output_options = algo.get_output_options();
    EXPECT_EQ(output_options.size(), 1);
    EXPECT_EQ(output_options[0], "STATS_FILE_NAME history.txt");
}

TEST(AlgorithmTest, InvalidInput) {
    std::string s = "Invalid input string";
    std::string error_msg;
    Algorithm algo(s, error_msg);
    EXPECT_EQ(algo.get_id(), "");
    EXPECT_EQ(algo.get_name(), "");
    EXPECT_TRUE(algo.get_output_options().empty());
}

// For now, we are not testing the following methods 
// because it seems that these functionalities are not
//  needed for the current implementation
// TEST(AlgorithmTest, CopyConstructor) {
//     std::string s = "Algo_1 (DAMON X: Default) [STATS_FILE_NAME history.txt]";
//     std::string error_msg;
//     RUNNERPOST::Algorithm algo1(s, error_msg);
//     RUNNERPOST::Algorithm algo2 = algo1;

//     EXPECT_EQ(algo2.get_id(), "Algo_1");
//     EXPECT_EQ(algo2.get_name(), "DAMON X: Default");
//     EXPECT_FALSE(algo2.get_output_options().empty());

//     std::vector<std::string> output_options = algo2.get_output_options();
//     EXPECT_EQ(output_options.size(), 1);
//     EXPECT_EQ(output_options[0], "STATS_FILE_NAME history.txt");
// }

// TEST(AlgorithmTest, AssignmentOperator) {
//     std::string s1 = "Algo_1 (DAMON X: Default) [STATS_FILE_NAME history.txt]";
//     std::string s2 = "Algo_2 (DAMON Y: Custom) [STATS_FILE_NAME custom.txt]";
//     std::string error_msg;
//     RUNNERPOST::Algorithm algo1(s1, error_msg);
//     RUNNERPOST::Algorithm algo2(s2, error_msg);

//     algo2 = algo1;

//     EXPECT_EQ(algo2.get_id(), "Algo_1");
//     EXPECT_EQ(algo2.get_name(), "DAMON X: Default");
//     EXPECT_FALSE(algo2.get_output_options().empty());

//     std::vector<std::string> output_options = algo2.get_output_options();
//     EXPECT_EQ(output_options.size(), 1);
//     EXPECT_EQ(output_options[0], "STATS_FILE_NAME history.txt");
// }

// TEST(AlgorithmTest, EqualityOperator) {
//     std::string s1 = "Algo_1 (DAMON X: Default) [STATS_FILE_NAME history.txt]";
//     std::string s2 = "Algo_1 (DAMON X: Default) [STATS_FILE_NAME history.txt]";
//     std::string error_msg;
//     RUNNERPOST::Algorithm algo1(s1, error_msg);
//     RUNNERPOST::Algorithm algo2(s2, error_msg);

//     EXPECT_TRUE(algo1 == algo2);
// }

// Add more tests as needed