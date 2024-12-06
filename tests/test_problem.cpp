#include <gtest/gtest.h>
#include "../src/Problem.hpp"

// use RUNNERPOST namespace
using namespace RUNNERPOST;

TEST(ProblemTest, DefaultConstructor) {

    // When the default constructor is called, the profile type, y select, and x select should be undefined
    std::string error_msg;
    Problem pb("", error_msg);
    EXPECT_EQ(pb.get_id(), "");
    EXPECT_EQ(pb.get_name(), "");
    EXPECT_EQ(pb.get_n(), -1);
    EXPECT_EQ(pb.get_m(), -1);
    EXPECT_TRUE(pb.get_pbInstance().empty());

}

// TODO