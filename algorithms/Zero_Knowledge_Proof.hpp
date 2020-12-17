#ifndef ZERO_KNOWLEDGE_PROOF_HPP
#define ZERO_KNOWLEDGE_PROOF_HPP

// Standard C/C++
#include <algorithm>
#include <stdlib.h>
#include <vector>

// Internal
#include "../utils/Math_utils.hpp"
#include "../utils/Crypto_utils.hpp"

namespace crypto
{
namespace algos
{
static inline bool
// Everyone knows Alice's Public Key
Zero_Knowledge_Proof(const mpz_class N,
                     const std::vector<mpz_class> PubKey,
                     const std::vector<mpz_class> PrivKey)
{
    assert(PubKey.size() == PrivKey.size());
    // Find Blum's primes: p, q that satisfy:
    //  N = pq
    //  p mod 4 = q mod 4 = 3 (Blum's primes)

    // Everyone knows the public ID
    
    // Satisfy:
    // PrivKey[i]^2 * PubKey[i] mod N = 1
    for (size_t i = 0; i < PubKey.size(); ++i)
    {
        mpz_class result{};
        mpz_class base = (PrivKey[i] * PrivKey[i]) * PubKey[i];
        mpz_powm_ui(result.get_mpz_t(), // Result buffer
                    base.get_mpz_t(), // Base (d * e)
                    1,                   // Exponent
                    N.get_mpz_t());      // Modulo)

        std::cout << "result = " << result << "\tPubKey = " << PubKey[i] << "\tPrivKey = " << PrivKey[i] << std::endl;
        assert(result == 1);
    }

    return true;
}

static inline bool
// Nobody else knows Alice's Private ID
Zero_Knowledge_Proof(const mpz_class N,
                     const std::vector<mpz_class> PubKey,
                     const std::vector<mpz_class> PrivKey,
                     const std::vector<mpz_class> random_numbers,
                     const std::vector<std::vector<size_t>> mat)
{
    // Random numbers from Alice needs to have the same number of columns
    assert(random_numbers.size() == mat.size());

    // Private key size needs to have the same number of rows
    assert(PrivKey.size() == mat[0].size());

    // Calculate:
    // random_numbers[i]^2 mod N = some answer
    std::vector<mpz_class> numbers_to_Bob;
    numbers_to_Bob.reserve(random_numbers.size());

    for (const auto& rand_num : random_numbers)
    {
        mpz_class new_num{};
        mpz_class base = rand_num * rand_num;
        mpz_powm_ui(new_num.get_mpz_t(), // Result buffer
                    base.get_mpz_t(),    // Base (d * e)
                    1,                   // Exponent
                    N.get_mpz_t());      // Modulo)

        numbers_to_Bob.emplace_back(new_num);
    }
    // Alice's Proof of Identity to Bob
    // Bob sends this matrix to Alice and Alice calculates new numbers
    std::vector<mpz_class> numbers_to_Bob_challenge;
    numbers_to_Bob_challenge.reserve(numbers_to_Bob.size());
    for (size_t i = 0; i < random_numbers.size(); ++i)
    {
        mpz_class result = random_numbers[i];
        for (size_t j = 0; j < mat[i].size(); ++j)
        {
            mpz_class rop{};
            mpz_ui_pow_ui(rop.get_mpz_t(), PrivKey[j].get_ui(), mat[i][j]);
            // std::cout << "PrivKey Entry: " << PrivKey[j] << std::endl;
            // std::cout << "Matrix Entry: " << mat[i][j] << std::endl;
            // std::cout << "operation: " << result << " * " << rop << std::endl;
            result *= rop;
        }

        mpz_class R{};
        mpz_class base(result);
        mpz_powm_ui(R.get_mpz_t(),  // Result buffer
                    base.get_mpz_t(),   // Base (d * e)
                    1,                  // Exponent
                    N.get_mpz_t()); // Modulo)

        numbers_to_Bob_challenge.emplace_back(R);
        // std::cout << "Number to Bob #" << i << ": " << R << std::endl;
        // std::cout << std::endl;
    }

    // Bob's Verification
    for (size_t i = 0; i < numbers_to_Bob_challenge.size(); ++i)
    {
        mpz_class result = numbers_to_Bob_challenge[i] * numbers_to_Bob_challenge[i];
        for (size_t j = 0; j < mat[i].size(); ++j)
        {
            mpz_class rop{};
            mpz_ui_pow_ui(rop.get_mpz_t(), PubKey[j].get_ui(), mat[i][j]);
            result *= rop;
        }

        mpz_class R{};
        mpz_class base(result);
        mpz_powm_ui(R.get_mpz_t(),  // Result buffer
                    base.get_mpz_t(),   // Base (d * e)
                    1,                  // Exponent
                    N.get_mpz_t()); // Modulo)

        if (R != numbers_to_Bob[i])
        {
            std::cout << "result: " << result << " != " << numbers_to_Bob_challenge[i] << std::endl;
            return false;
        }
        assert(R == numbers_to_Bob[i]);
    }
    return true;
    // assert(check_1 == 1);

    // mpz_class check_2{};
    // mpz_class base_2 = (PrivKey.second * PrivKey.second) * PubKey.second;
    // mpz_powm_ui(check_2.get_mpz_t(),  // Result buffer
    //             base_2.get_mpz_t(),   // Base (d * e)
    //             1,                  // Exponent
    //             N.get_mpz_t()); // Modulo)

    // assert(check_2 == 1);

    // return (check_1 == 1 && check_2 == 1);
}

} // algos
} // crypto

#endif // ZERO_KNOWLEDGE_PROOF_HPP