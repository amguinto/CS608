#include "../algorithms/RSA.hpp"

// Standard C/C++
#include <iostream>

// Google
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(test_RSA, RSA_Key_Generation)
{
    auto keys = crypto::algos::RSA_Key_Generation(23,  // random prime p 
                                                  37,  // random prime q
                                                  53); // small number e, coprime to phi_n
    auto e = keys.first.first;
    auto n = keys.first.second;
    auto d = keys.second;

    EXPECT_EQ(e, 53);
    EXPECT_EQ(n, 851);
    EXPECT_EQ(d, 269);

    auto keys2 = crypto::algos::RSA_Key_Generation(7,  // random prime p 
                                                   19,  // random prime q
                                                   5); // small number e, coprime to phi_n
    auto e2 = keys2.first.first;
    auto n2 = keys2.first.second;
    auto d2 = keys2.second;

    EXPECT_EQ(e2, 5);
    EXPECT_EQ(n2, 133);
    EXPECT_EQ(d2, 65);
}

TEST(test_RSA, RSA_Cryptosystem)
{
    auto keys2 = crypto::algos::RSA_Key_Generation(7,  // random prime p 
                                                   19, // random prime q
                                                   5); // small number e, coprime to phi_n
    auto e2 = keys2.first.first;
    auto n2 = keys2.first.second;
    auto d2 = keys2.second;

    auto ciphertext = crypto::algos::RSA_Encrypt(6,
                                                 e2,
                                                 n2);
    EXPECT_EQ(ciphertext, 62);

    auto message = crypto::algos::RSA_Decrypt(ciphertext,
                                              d2,
                                              n2);
    EXPECT_EQ(message, 6);
}
