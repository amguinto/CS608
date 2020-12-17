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
#ifndef using_ASCII
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
#endif
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

// TEST(test_RSA, Exam_Cryptosystem)
// {

//     // Option to choose a number or text.
//     mpz_class p_a = 1300027;
//     mpz_class message = p_a * (p_a + mpz_class(1));
//     std::cout << "message = " << message << std::endl;
//     // std::string plaintext = "crypto";
//     // auto naive = message::naive_plaintext_numeric(plaintext); // Find sweet spot for block size.
//     // auto message = message::encode_naive_representation(naive); // compress

//     // std::cout << "message = " << message << std::endl;

//     // IMPORTANT: If we have multiple blocks, just loop.
//     // for (const auto& block : message)
//     // {
//     //     std::cout << "plaintext block = ";
//     //     std::cout << block << "\t";
//     // }
//     // std::cout << std::endl;

//     /************KEY GENERATION************
//     **************************************/
//     // Step 1: Generate 2 large primes p, q, and let n = p * q;
//     mpz_class p(1301017);
//     mpz_class q(1300051);
//     mpz_class n(p * q);
//     std::cout << "n = " << n << std::endl;    
//     std::cout << "p = " << p << std::endl;
//     std::cout << "q = " << q << std::endl;

//     // Choose a small number e, coprime to phi(n). (Assertion handled in key gen function)
//     mpz_class e(11);


//     // {e, n} => Public keys
//     // {d}    => Private keys
//     auto PKeys = crypto::algos::RSA_Key_Generation(p,
//                                                    q,
//                                                    e);

//     /*************ENCRYPTION***************
//     **************************************/
//     // c = m^e mod n
//     auto ciphertext = crypto::algos::RSA_Encrypt(message,
//                                                  e,
//                                                  n);
//     std::cout << "ciphertext = " << ciphertext << std::endl;


//     // IMPORTANT: If we have multiple blocks, just loop.
//     // std::vector<mpz_class> encrypted_blocks;
//     // for (const auto& block : message)
//     // {
//     //     encrypted_blocks.emplace_back(crypto::algos::RSA_Encrypt(block, e, n));
//     // }

//     // for (const auto& block : encrypted_blocks)
//     // {
//     //     std::cout << "encrypted block = ";
//     //     std::cout << block << "\t";
//     // }
//     // std::cout << std::endl;

//     /*************DECRYPTION***************
//     **************************************/
//     // m = c^d mod n
//     auto decryption = crypto::algos::RSA_Decrypt(ciphertext,
//                                                  PKeys.second, // Private key (d)
//                                                  n);
//     std::cout << "decryption = " << decryption << std::endl;
//     EXPECT_EQ(message, decryption);


//     // IMPORTANT: If we have multiple blocks, just loop.
//     // std::vector<mpz_class> decrypted_blocks;
//     // for (const auto& block : encrypted_blocks)
//     // {
//     //     decrypted_blocks.emplace_back(crypto::algos::RSA_Decrypt(block, PKeys.second, n));
//     // }

//     // for (const auto& block : decrypted_blocks)
//     // {
//     //     std::cout << "decrypted block = ";
//     //     std::cout << block << "\t";
//     // }
//     // std::cout << std::endl;

//     // for (const auto& block : decrypted_blocks)
//     // {
//     //     std::cout << "plaintext = \t";
//     //     std::cout << message::numeric_to_char(block);
//     // }
//     // std::cout << std::endl;
// }

// TEST(test_RSA, Exam_Digital_Signature)
// {
//     // Choose Sender parameters.
//     mpz_class p(3659);     // prime #
//     mpz_class q(823);    // prime #
//     mpz_class m(p * q); // used as the modulus in signing/verification
//     mpz_class e(5);    // choose a coprime to phi(m) (NOTE: modular inverse to d (private key of the sender))
//                         // Also used as the exponent in verification

//     mpz_class r(2063);    // prime #
//     mpz_class s(2593);    // prime #
//     mpz_class n(r * s); // used as the modulus in encryption/decryption
//     mpz_class h(5);    // choose a coprime to phi(m) (NOTE: modular inverse to g (private key of the sender))

//     // mpz_class message(72); // Some message we want.

//     // Alternatively we can specify a message.
//     std::string plaintext = "njit";
//     auto numeric_form = message::naive_plaintext_numeric(plaintext);
//     auto message = message::encode_naive_representation(numeric_form);
//     // Get Public keys.

//     // Pair: {m, e} or {n, h} => public keys
//     //       {d} or {g}   => private keys
//     auto sender = crypto::algos::RSA_Key_Generation_DS(p,
//                                                        q,
//                                                        e);

//     std::cout << "Sender params: " << std::endl;
//     std::cout << "first param in public key (m) = " << sender.first.first << std::endl;
//     std::cout << "second param in public key (e) = " << sender.first.second << std::endl;
//     std::cout << "private key (d) = " << sender.second << std::endl;
//     std::cout << std::endl;
//     std::cout << std::endl;
//     EXPECT_EQ(sender.second, 2405501);

//     auto receiver = crypto::algos::RSA_Key_Generation_DS(r,
//                                                          s,
//                                                          h);

//     std::cout << "Receiver params: " << std::endl;
//     std::cout << "first param in public key (n) = " << receiver.first.first << std::endl;
//     std::cout << "second param in public key (h) = " << receiver.first.second << std::endl;
//     std::cout << "private key (g) = " << receiver.second << std::endl;
//     std::cout << std::endl;
//     std::cout << std::endl;
//     EXPECT_EQ(receiver.second, 1068941);

//     // Just make an easier alias to the pairs: {m, e} for sender and {n, h} for receiever
//     auto PK_sender = std::make_pair(sender.first.first, sender.first.second);
//     auto PK_receiver = std::make_pair(receiver.first.first, receiver.first.second);

//     // Get the signed message
//     auto signed_message = crypto::algos::RSA_Sign_and_Encrypt(sender.second, // Sender private key
//                                                               m,
//                                                               n,
//                                                               PK_receiver,
//                                                               message);      // numeric (or compressed) form of message

//     std::cout << "message = " << message << std::endl;
//     std::cout << "signed message = " << signed_message << std::endl;
//     EXPECT_EQ(signed_message, 3157055);

//     auto verified_message = crypto::algos::RSA_Decrypt_and_Verify(receiver.second, // Receiver private key
//                                                                   m,
//                                                                   n,
//                                                                   PK_sender,
//                                                                   signed_message);
//     std::cout << "verified_message = " << verified_message << std::endl;
//     EXPECT_EQ(verified_message, message);
//     std::cout << "plaintext = " << message::decode_naive_representation(message) << std::endl;
// }
