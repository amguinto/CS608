#include "../algorithms/Algorithms.hpp"

// Standard C/C++
#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(tests_Algorithms, Get_Public_Keys)
{
    auto PKeys = algos::Get_Public_Keys(51,
                                        92,
                                        227,
                                        2);
    EXPECT_EQ(PKeys.first.first,  96);
    EXPECT_EQ(PKeys.first.second, 9);
}

TEST(test_Algorithms, Diffie_Hellman_Key_Exchange)
{
    auto SK_1 = algos::Diffie_Hellman_Key_Exchange(51, 92, 227);
    EXPECT_EQ(SK_1, 167);

    auto SK_2 = algos::Diffie_Hellman_Key_Exchange(25, 157, 227);
    EXPECT_EQ(SK_2, 98);
}

TEST(test_Algorithms, ElGamal_Cryptosystem)
{
    // Homework assignment Problem.
    const auto encryption = algos::ElGamal_Encryption(29,  // a
                                                      23,  // b
                                                      89,  // modulo (p)
                                                      57,  // A's secret key
                                                      72); // message

    EXPECT_EQ(encryption.first.first,  56);  // Ciphertext
    EXPECT_EQ(encryption.first.second, 23); // Hint
    EXPECT_EQ(encryption.second.first, 89); // Modulo
    EXPECT_EQ(encryption.second.second, 3); // Generator

    const auto decryption = algos::ElGamal_Decryption(29, // a
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
    // auto pair2 = algos::ElGamal_Encryption(9,   // a
    //                                        13,  // b
    //                                        53,  // modulo (p)
    //                                        8,   // A's secret key
    //                                        21); // message

    // EXPECT_EQ(pair2.first, 47);  // Ciphertext
    // EXPECT_EQ(pair2.second, 45); // Hint

    // auto decrypted2 = algos::ElGamal_Decryption(9,      // a
    //                                             13,     // b
    //                                             53,     // modulo (p)
    //                                             pair2); // {Ciphertext, Hint}
    // EXPECT_EQ(decrypted2, 21);
}