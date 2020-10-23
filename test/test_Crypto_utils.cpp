#include "../utils/Crypto_utils.hpp"

// Standard C/C++
#include <iostream>

// Google
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(tests_Algorithms, Get_Public_Keys)
{
    auto PKeys = crypto::utils::Get_Public_Keys(51,
                                                92,
                                                227,
                                                2);
    EXPECT_EQ(PKeys.first.first,  96);
    EXPECT_EQ(PKeys.first.second, 9);
}