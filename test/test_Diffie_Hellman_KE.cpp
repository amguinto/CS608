#include "../algorithms/Diffie_Hellman_KE.hpp"

// Standard C/C++
#include <iostream>

// Google
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(test_Diffie_Hellman_KE, Diffie_Hellman_Key_Exchange)
{
    auto SK_1 = crypto::algos::Diffie_Hellman_Key_Exchange(51, 92, 227);
    EXPECT_EQ(SK_1, 167);

    auto SK_2 = crypto::algos::Diffie_Hellman_Key_Exchange(25, 157, 227);
    EXPECT_EQ(SK_2, 98);
}
