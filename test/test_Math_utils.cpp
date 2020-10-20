#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../utils/Math_utils.hpp"


TEST(test_Message_utils, find_smallest_generator)
{
    EXPECT_EQ(math::find_smallest_generator(307), 5);
}

TEST(test_Message_utils, is_generator)
{
    EXPECT_FALSE(math::is_generator(307, 2));
    EXPECT_FALSE(math::is_generator(307, 3));
    EXPECT_FALSE(math::is_generator(307, 4));
    EXPECT_TRUE(math::is_generator(307, 5));
}