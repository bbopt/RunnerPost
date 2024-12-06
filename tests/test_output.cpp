#include <gtest/gtest.h>
#include "../src/Output.hpp"

// use RUNNERPOST namespace
using namespace RUNNERPOST;

TEST(OutputTest, DefaultConstructor) {

    // When the default constructor is called, the profile type, y select, and x select should be undefined
    std::string error_msg;
    Output output("", error_msg);
    EXPECT_EQ(output.get_profile_type(), Output::Profile_Type::UNDEFINED);
    EXPECT_EQ(output.get_y_select(), Output::Y_Select::OBJ);
    EXPECT_EQ(output.get_x_select(), Output::X_Select::NP1EVAL);
}

TEST(OutputTest, SetProfileType) {
    std::string error_msg;
    Output output("", error_msg);
    output.setProfileType(Output::Profile_Type::DATA_PROFILE);
    EXPECT_EQ(output.get_profile_type(), Output::Profile_Type::DATA_PROFILE);

    output.setProfileType(Output::Profile_Type::PERFORMANCE_PROFILE);
    EXPECT_EQ(output.get_profile_type(), Output::Profile_Type::PERFORMANCE_PROFILE);

    output.setProfileType(Output::Profile_Type::HISTORY);
    EXPECT_EQ(output.get_profile_type(), Output::Profile_Type::HISTORY);

    output.setProfileType(Output::Profile_Type::ACCURACY_PROFILE);
    EXPECT_EQ(output.get_profile_type(), Output::Profile_Type::ACCURACY_PROFILE);

    output.setProfileType(Output::Profile_Type::UNDEFINED);
    EXPECT_EQ(output.get_profile_type(), Output::Profile_Type::UNDEFINED);
}

TEST(OutputTest, SetYSelect) {
    std::string error_msg;
    Output output("", error_msg);
    output.setYSelect(Output::Y_Select::OBJ);
    EXPECT_EQ(output.get_y_select(), Output::Y_Select::OBJ);

    output.setYSelect(Output::Y_Select::INFEAS);
    EXPECT_EQ(output.get_y_select(), Output::Y_Select::INFEAS);
}

TEST(OutputTest, SetXSelect) {
    std::string error_msg;
    Output output("", error_msg);
    output.setXSelect(Output::X_Select::TIME);
    EXPECT_EQ(output.get_x_select(), Output::X_Select::TIME);

    output.setXSelect(Output::X_Select::EVAL);
    EXPECT_EQ(output.get_x_select(), Output::X_Select::EVAL);
}