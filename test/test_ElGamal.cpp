#include "../algorithms/ElGamal.hpp"

// Standard C/C++
#include <iostream>

// Google
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(test_ElGamal, ElGamal_Cryptosystem)
{
    // Homework assignment Problem.
    const auto encryption = crypto::algos::ElGamal_Encrypt(29,  // a
                                                              23,  // b
                                                              89,  // modulo (p)
                                                              57,  // A's secret key
                                                              72); // message

    EXPECT_EQ(encryption.first.first,  56); // Ciphertext
    EXPECT_EQ(encryption.first.second, 23); // Hint
    EXPECT_EQ(encryption.second.first, 89); // Modulo
    EXPECT_EQ(encryption.second.second, 3); // Generator

    const auto decryption = crypto::algos::ElGamal_Decrypt(29, // a
                                                              23, // b
                                                              encryption); // {Ciphertext, Hint}, 
                                                                           // {Modulo, Generator}
    EXPECT_EQ(decryption, 72);

    // TODO: Test uing text.

/**********************************************************
 **********************************************************
 **********************************************************
*/ 
    // Edge case when we have a bad Mask.
    // We need to choose a different k if that is the case.
    auto bad_encryption = crypto::algos::ElGamal_Encrypt(9,   // a
                                                            13,  // b
                                                            53,  // modulo (p)
                                                            8,   // A's secret key
                                                            21); // message

    EXPECT_EQ(bad_encryption.first.first,  0);  // Ciphertext
    EXPECT_EQ(bad_encryption.first.second, 0);  // Hint
    EXPECT_EQ(bad_encryption.second.first, 53); // Modulo
    EXPECT_EQ(bad_encryption.second.second, 2); // Generator

    auto bad_decryption = crypto::algos::ElGamal_Decrypt(9,  // a
                                                            13, // b
                                                            bad_encryption); // {Ciphertext, Hint},
                                                                             // {Modulo, Generator}
    EXPECT_EQ(bad_decryption, 0);
}
