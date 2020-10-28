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
    auto e = keys.first.first; // small number e
    auto n = keys.first.second; // P * Q
    auto d = keys.second; // private key

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
    auto e2 = keys2.first.first; // small number e
    auto n2 = keys2.first.second; // P * Q
    auto d2 = keys2.second; // private key

    auto ciphertext = crypto::algos::RSA_Encrypt(6,
                                                 e2,
                                                 n2);
    EXPECT_EQ(ciphertext, 62);

    auto message = crypto::algos::RSA_Decrypt(ciphertext,
                                              d2,
                                              n2); // P * Q
    EXPECT_EQ(message, 6);
}

TEST(test_RSA, RSA_Sign_Encrypt_and_Decrypt_Verify_Large)
{
    mpz_class p(104801);
    mpz_class q(104803);
    mpz_class m(p * q);
    mpz_class e(23);

    mpz_class r(104827);
    mpz_class s(104831);
    mpz_class n(r * s);
    mpz_class h(37);

    // Get Public keys.
    auto sender = crypto::algos::RSA_Key_Generation_DS(p,
                                                       q,
                                                       e);

    auto receiver = crypto::algos::RSA_Key_Generation_DS(r,
                                                         s,
                                                         h);

    // Public keys of sender.
    auto PK_sender = std::make_pair(sender.first.first,
                                    sender.first.second);
    // Public key of receiver.
    auto PK_receiver = std::make_pair(receiver.first.first,
                                      receiver.first.second);

    // Get compressed numeric representation of "crypto". Using the older version without blocks.
    std::string plaintext = "crypto";
    auto numeric_form = message::naive_plaintext_numeric(plaintext);
    auto compressed_form = message::encode_naive_representation(numeric_form);
    // std::cout << "crypto = " << compressed_form << std::endl;

    mpz_class message(compressed_form);
    mpz_class signed_message = crypto::algos::RSA_Sign_and_Encrypt(sender.second,
                                                                   m,
                                                                   n,
                                                                   PK_receiver,
                                                                   message);
    // std::cout << "signed_message = " << signed_message << std::endl;

    auto verified_message = crypto::algos::RSA_Decrypt_and_Verify(receiver.second,
                                                                  m,
                                                                  n,
                                                                  PK_sender,
                                                                  signed_message);

    // std::cout << "verified_message = " << verified_message << std::endl;
}
TEST(test_RSA, RSA_Sign_Encrypt_and_Decrypt_Verify_Simple)
{
    mpz_class p(7);
    mpz_class q(13);
    mpz_class m(p * q);
    mpz_class e(23);

    mpz_class r(11);
    mpz_class s(17);
    mpz_class n(r * s);
    mpz_class h(37);

    mpz_class message(72);

    // Get Public keys.
    auto sender = crypto::algos::RSA_Key_Generation_DS(p,
                                                       q,
                                                       e);
    EXPECT_EQ(sender.second, 47);
    auto receiver = crypto::algos::RSA_Key_Generation_DS(r,
                                                         s,
                                                         h);
    EXPECT_EQ(receiver.second, 13);

    auto PK_sender = std::make_pair(sender.first.first, sender.first.second);
    auto PK_receiver = std::make_pair(receiver.first.first, receiver.first.second);

    auto signed_message = crypto::algos::RSA_Sign_and_Encrypt(sender.second,
                                                              m,
                                                              n,
                                                              PK_receiver,
                                                              message);
    EXPECT_EQ(signed_message, 67);

    auto verified_message = crypto::algos::RSA_Decrypt_and_Verify(receiver.second,
                                                                  m,
                                                                  n,
                                                                  PK_sender,
                                                                  signed_message);
    EXPECT_EQ(verified_message, message);
}