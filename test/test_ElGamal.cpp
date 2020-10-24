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

TEST(test_ElGamal, ElGamal_Key_Generation)
{
    auto key = crypto::algos::ElGamal_Key_Generation(8, 11);
    EXPECT_EQ(key.first, 3);          // key
    EXPECT_EQ(key.second.first, 2);   // generator
    EXPECT_EQ(key.second.second, 11); // modulo
}

TEST(test_ElGamal, ElGamal_Signing)
{
    auto signing = crypto::algos::ElGamal_Sign(9,  // Different Secret key R
                                               8,  // Secret key used in key generation, r
                                               5,  // message
                                               11, // modulo
                                               2); // generator
    EXPECT_EQ(signing.first.first, 5);   // message
    EXPECT_EQ(signing.first.second, 3); // Public key of sender
    EXPECT_EQ(signing.second.first, 6);  // X
    EXPECT_EQ(signing.second.second, 3); // Y
}

TEST(test_ElGamal, ElGamal_Verification)
{
    auto verification = crypto::algos::ElGamal_Verify(3,  // Public key of sender
                                                      6,  // X
                                                      3,  // Y
                                                      5,  // message
                                                      11, // modulo
                                                      2); // generator
    EXPECT_TRUE(verification);
}