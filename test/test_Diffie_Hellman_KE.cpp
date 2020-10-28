#include "../algorithms/Diffie_Hellman_KE.hpp"

// Standard C/C++
#include <iostream>

// Google
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// TEST(test_Diffie_Hellman_KE, Diffie_Hellman_Key_Exchange)
// {
//     auto SK_1 = crypto::algos::Diffie_Hellman_Key_Exchange(51, 92, 227);
//     EXPECT_EQ(SK_1, 167);

//     auto SK_2 = crypto::algos::Diffie_Hellman_Key_Exchange(25, 157, 227);
//     EXPECT_EQ(SK_2, 98);
// }

TEST(test_Diffie_Hellman_KE, Exam)
{
    // Exchange a secret key over an insecure channel without transmitting the key over the channel.
    // Step 1: Get the modulo (prime p) and the generator
    auto modulo = 227;
    auto gen = math::find_smallest_generator(227);

    // Step 2: Choose their secret keys a, b
    auto a = 51;
    auto b = 92;

    auto shared_key = crypto::algos::Diffie_Hellman_Key_Exchange(a, // private key of A
                                                                 b, // private key of B
                                                                 modulo);
    std::cout << "shared_key = " << shared_key << std::endl;

    EXPECT_EQ(shared_key, 167);
}