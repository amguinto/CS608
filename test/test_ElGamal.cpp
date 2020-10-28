#include "../algorithms/ElGamal.hpp"

// Standard C/C++
#include <iostream>

// Google
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// TEST(test_ElGamal, ElGamal_Cryptosystem)
// {
//     // Homework assignment Problem.
//     const auto encryption = crypto::algos::ElGamal_Encrypt(29,  // a
//                                                               23,  // b
//                                                               89,  // modulo (p)
//                                                               57,  // A's secret key
//                                                               72); // message

//     EXPECT_EQ(encryption.first.first,  56); // Ciphertext
//     EXPECT_EQ(encryption.first.second, 23); // Hint
//     EXPECT_EQ(encryption.second.first, 89); // Modulo
//     EXPECT_EQ(encryption.second.second, 3); // Generator

//     const auto decryption = crypto::algos::ElGamal_Decrypt(29, // a
//                                                               23, // b
//                                                               encryption); // {Ciphertext, Hint}, 
//                                                                            // {Modulo, Generator}
//     EXPECT_EQ(decryption, 72);

//     // TODO: Test uing text.

// /**********************************************************
//  **********************************************************
//  **********************************************************
// */ 
//     // Edge case when we have a bad Mask.
//     // We need to choose a different k if that is the case.
//     auto bad_encryption = crypto::algos::ElGamal_Encrypt(9,   // a
//                                                          13,  // b
//                                                          53,  // modulo (p)
//                                                          8,   // A's secret key
//                                                          21); // message

//     EXPECT_EQ(bad_encryption.first.first,  0);  // Ciphertext
//     EXPECT_EQ(bad_encryption.first.second, 0);  // Hint
//     EXPECT_EQ(bad_encryption.second.first, 53); // Modulo
//     EXPECT_EQ(bad_encryption.second.second, 2); // Generator

//     auto bad_decryption = crypto::algos::ElGamal_Decrypt(9,  // a
//                                                          13, // b
//                                                          bad_encryption); // {Ciphertext, Hint},
//                                                                           // {Modulo, Generator}
//     EXPECT_EQ(bad_decryption, 0);
// }

// TEST(test_ElGamal, ElGamal_Key_Generation)
// {
//     auto key = crypto::algos::ElGamal_Key_Generation(8, 11);
//     EXPECT_EQ(key.first, 3);          // key
//     EXPECT_EQ(key.second.first, 2);   // generator
//     EXPECT_EQ(key.second.second, 11); // modulo
// }

// TEST(test_ElGamal, ElGamal_Signing)
// {
//     auto signing = crypto::algos::ElGamal_Sign(9,  // Different Secret key R
//                                                8,  // Secret key used in key generation, r
//                                                5,  // message
//                                                11, // modulo
//                                                2); // generator
//     EXPECT_EQ(signing.first.first, 5);   // message
//     EXPECT_EQ(signing.first.second, 3); // Public key of sender
//     EXPECT_EQ(signing.second.first, 6);  // X
//     EXPECT_EQ(signing.second.second, 3); // Y
// }

// TEST(test_ElGamal, ElGamal_Verification)
// {
//     //
//     auto verification = crypto::algos::ElGamal_Verify(3,  // Public key of sender
//                                                       6,  // X
//                                                       3,  // Y
//                                                       5,  // message
//                                                       11, // modulo
//                                                       2); // generator
//     EXPECT_TRUE(verification);
// }

TEST(test_ElGamal, Exam_Cryptosystem)
{
    /************KEY GENERATION************
    **************************************/
    // Step 1: All users agree on a large prime p (modulo) and a generator g
    auto modulo = 104801;
    auto gen = math::find_smallest_generator(modulo); // Can be given to us.

    // Step 2: All users A, B select their private keys a, b and calculate public keys.
    auto a = 59;
    auto b = 113;
    auto k = 25; // NOTE: This is the second private key of A. If not specified, use a = k.
    // PK_A = g^a mod p;
    // {key}, {generator, modulo}
    auto PK_A = crypto::algos::ElGamal_Key_Generation(a,       // Private key
                                                      modulo); // Modulo

    std::cout << "Public key of A = " << PK_A.first << std::endl;

    // PK_B = g^b mod p;
    // {key}, {generator, modulo}
    auto PK_B = crypto::algos::ElGamal_Key_Generation(b,       // Private key
                                                      modulo); // Modulo
    std::cout << "Public key of B = " << PK_B.first << std::endl;

    /**************ENCRYPTION**************
    **************************************/
    auto message = 898;
    // Step 1: Calculate Ciphertext and Hint to be sent to B.
    // NOTE: We calculate the public keys again just, so we pass in private keys.
    auto send_package = crypto::algos::ElGamal_Encrypt(a, // private key of a
                                                       b, // private key of b 
                                                       modulo,
                                                       k,
                                                       message);
    std::cout << "ciphertext = " << send_package.first.first << std::endl;
    std::cout << "hint = " << send_package.first.second << std::endl;
    std::cout << "modulo = " << send_package.second.first << std::endl;
    std::cout << "generator = " << send_package.second.second << std::endl;

    /**************DECRYPTION**************
    **************************************/
    auto decryption = crypto::algos::ElGamal_Decrypt(a, // private key of a
                                                     b, // private key of b
                                                     send_package); // Package from A
    std::cout << "message = " << message << std::endl;
    std::cout << "decryption = " << decryption << std::endl;
    
    // We know this works when the decryption = message
    EXPECT_EQ(decryption, message);
}

TEST(test_ElGamal, Exam_Digital_Signature)
{
    auto message = 17;
    /************KEY GENERATION************
    **************************************/
    // Step 1: Select prime p as the modulo and get the generator
    auto modulo = 104801;
    auto generator = math::find_smallest_generator(modulo);

    // Step 2: Sender selects a random integer r (secret key) s.t. 0 < r < p - 1
    auto r = 7;
    // Calculate the public key: K = g^r mod p
    auto public_key = crypto::algos::ElGamal_Key_Generation(r, // r
                                                            modulo); // K = (g^r) mod p
    std::cout << "public key K = " << public_key.first << "\t";
    std::cout << "generator = " << public_key.second.first << "\t";
    std::cout << "modulo = " << public_key.second.second << std::endl;

    // public_key = {secret key r}, {generator, modulo} and is known to the public.
    EXPECT_EQ(public_key.first, 77519);            // public key
    EXPECT_EQ(public_key.second.first, generator); // generator
    EXPECT_EQ(public_key.second.second, modulo);   // modulo


    /**************SIGNING*****************
    **************************************/
    // Step 1: The sender selects another random integer R s.t. 0 < R < p - 1 and coprime to (p - 1) 
    auto R = 9;
    auto signing = crypto::algos::ElGamal_Sign(R,          // Different Secret key R
                                               r,          // (r) Secret key used in key generation, r
                                               message,    // message
                                               modulo,     // modulo
                                               generator); // generator

    // signing = {message, Public Key}, {X, Y}
    // {X, Y} is the signature of message M
    EXPECT_EQ(signing.first.first, 17);  // message
    EXPECT_EQ(signing.first.second, 77519);  // Public key of sender
    // EXPECT_EQ(signing.second.first, 6);  // X (NOTE: This is very large.)
    // EXPECT_EQ(signing.second.second, 3); // Y (NOTE: This is very large.)

    /************VERIFICATION**************
    **************************************/
   // The receiver needs {Message, X, Y} for its computation
   // The public keys, generator, and modulo are assumed to be known.
    auto verification = crypto::algos::ElGamal_Verify(signing.first.second,  // Public key of sender
                                                      signing.second.first,  // X
                                                      signing.second.second, // Y
                                                      signing.first.first,   // message
                                                      modulo,                // modulo
                                                      generator);            // generator
    // Returns true if the calculation for A == g^Message
    EXPECT_TRUE(verification);
}