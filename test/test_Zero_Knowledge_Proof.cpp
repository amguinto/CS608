#include "../algorithms/Zero_Knowledge_Proof.hpp"

// Standard C/C++
#include <algorithm>
#include <iostream>

// Google
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(test_Zero_Knowledge_Proof, Everyone_knows_pub_key)
{
    // Zero_Knowledge_Proof(const mpz_class N,
                    //  const std::pair<mpz_class, mpz_class> PubKey,
                    //  const std::pair<mpz_class, mpz_class> PrivKey)
    auto satisfied = crypto::algos::Zero_Knowledge_Proof
                                            (77, 
                                             std::vector<mpz_class>{mpz_class(58), // PubKey
                                                                    mpz_class(67)},
                                             std::vector<mpz_class>{mpz_class(9),  // PrivKey
                                                                    mpz_class(10)});
    EXPECT_TRUE(satisfied);
}

TEST(test_Zero_Knowledge_Proof, Nobody_knows_priv_key)
{
    auto rows = 3;
    auto cols = 2;
    std::vector<std::vector<size_t> > mat(rows, std::vector<size_t>(cols));    

    // Row 1
    mat[0][0] = 0;
    mat[0][1] = 1;

    // Row 2
    mat[1][0] = 1;
    mat[1][1] = 0;

    // Row 3
    mat[2][0] = 1;
    mat[2][1] = 1;

    auto satisfied = crypto::algos::Zero_Knowledge_Proof
                                            (77,
                                            std::vector<mpz_class>{mpz_class(58),  // PubKey
                                                                   mpz_class(67)},
                                            std::vector<mpz_class>{mpz_class(9),   // PrivKey
                                                                   mpz_class(10)},
                                             std::vector<mpz_class>{mpz_class(19), // Random Numbers
                                                                    mpz_class(24),
                                                                    mpz_class(51)},
                                             mat);
    EXPECT_TRUE(satisfied);
}

TEST(test_Zero_Knowledge_Proof, Exam)
{
    auto rows = 3;
    auto cols = 2;
    std::vector<std::vector<size_t> > mat(rows, std::vector<size_t>(cols));    

    // Row 1
    mat[0][0] = 0;
    mat[0][1] = 1;

    // Row 2
    mat[1][0] = 1;
    mat[1][1] = 0;

    // Row 3
    mat[2][0] = 1;
    mat[2][1] = 1;

    auto satisfied = crypto::algos::Zero_Knowledge_Proof
                            (77,
                            std::vector<mpz_class>{mpz_class(58),  // PubKey
                                                   mpz_class(67)},
                            std::vector<mpz_class>{mpz_class(9),   // PrivKey
                                                   mpz_class(10)},
                            std::vector<mpz_class>{mpz_class(19), // Random Numbers
                                                   mpz_class(24),
                                                   mpz_class(51)},
                            mat);
    EXPECT_TRUE(satisfied);
}