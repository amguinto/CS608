#include "../algorithms/Menezes_Vanstone.hpp"

// Standard C/C++
#include <iostream>

// Google
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(test_Diffie_Hellman_KE, Menezes_Vanstone_EC)
{
    mpz_class modulo = 11;
    mpz_class a = 1;

    // Step 2: Choose their secret keys a, b
    mpz_class Alice_Priv_Key = 6;
    mpz_class Bob_Priv_Key = 8;

    // NOTE: A and k are the same number.
    mpz_class k = 6;
    std::pair<mpz_class, mpz_class> generator = std::make_pair(mpz_class(2), mpz_class(7));

    auto plaintext = std::make_pair(mpz_class(9), mpz_class(1));

    auto pt = crypto::algos::Menezes_Vanstone_EC(Alice_Priv_Key,
                                                 Bob_Priv_Key,
                                                 generator,
                                                 modulo,
                                                 a,
                                                 k,
                                                 plaintext);

    std::cout << "Plaintext = (" << pt.first << ", " << pt.second << ")" << std::endl;
}
