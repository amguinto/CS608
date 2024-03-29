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

TEST(test_Diffie_Hellman_KE, Exam)
{
    // Exchange a secret key over an insecure channel without transmitting the key over the channel.
    // Step 1: Get the modulo (prime p) and the generator
    auto modulo = 1300027;
    auto gen = math::find_smallest_generator(1300027);

    // Step 2: Choose their secret keys a, b
    auto a = math::IPow(23, 2);
    auto b = math::IPow(23, 3);
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    auto shared_key = crypto::algos::Diffie_Hellman_Key_Exchange(a.get_ui(), // private key of A
                                                                 b.get_ui(), // private key of B
                                                                 modulo);
    std::cout << "shared_key = " << shared_key << std::endl;

    EXPECT_EQ(shared_key, 775936);
}

TEST(test_Diffie_Hellman_KE, Diffie_Hellman_Key_Exchange_EC)
{
    mpz_class modulo = 23;
    mpz_class a = 2;

    // Step 2: Choose their secret keys a, b
    mpz_class Alice_Priv_Key = 3;
    mpz_class Bob_Priv_Key = 11;

    std::pair<mpz_class, mpz_class> generator = std::make_pair(mpz_class(0), mpz_class(3));

    auto shared_key = crypto::algos::Diffie_Hellman_Key_Exchange_EC(Alice_Priv_Key,
                                                                    Bob_Priv_Key,
                                                                    generator,
                                                                    modulo,
                                                                    a);
    std::cout << "Shared Key = (" << shared_key.first << ", " << shared_key.second << ")" << std::endl;
}
