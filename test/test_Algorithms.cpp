#include "../algorithms/Algorithms.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(test_Algorithms, Diffie_Hellman_Key_Exchange)
{
    auto SK_1 = algos::Diffie_Hellman_Key_Exchange(51, 92, 227);
    EXPECT_EQ(SK_1, 167);

    auto SK_2 = algos::Diffie_Hellman_Key_Exchange(25, 157, 227);
    EXPECT_EQ(SK_2, 98);
}