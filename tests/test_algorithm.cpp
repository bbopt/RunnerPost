
// tests/test_algorithm.cpp
#include <gtest/gtest.h>
#include "Algorithm.hpp"

TEST(AlgorithmTest, ResetInfo) {
    RUNNERPOST::Algorithm algo;
    algo.resetInfo();
    EXPECT_EQ(algo.getId(), "");
    EXPECT_EQ(algo.getNameAndExtra(), "");
    EXPECT_TRUE(algo.getOutputOptions().empty());
}

TEST(AlgorithmTest, Constructor) {
    std::string s = "Algo_1 (DAMON X: Default) [STATS_FILE_NAME history.txt][STATS_FILE_OUTPUT SOL OBJ CST ]  [ADD_PBINSTANCE_TO_STATS_FILE yes]";
    std::string error_msg;
    RUNNERPOST::Algorithm algo(s, error_msg);
    EXPECT_EQ(algo.getId(), "Algo_1");
    EXPECT_EQ(algo.getNameAndExtra(), "(DAMON X: Default)");
    EXPECT_FALSE(algo.getOutputOptions().empty());
}

// Add more tests as needed