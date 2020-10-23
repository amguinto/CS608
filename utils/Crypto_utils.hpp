#ifndef CRYPTO_UTILS_HPP
#define CRYPTO_UTILS_HPP

// Standard C/C++
#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>

// Internal
#include "../utils/Math_utils.hpp"
#include "../utils/Message_utils.hpp"

namespace crypto
{
namespace utils
{
static inline std::pair<std::pair<mpz_class, mpz_class>, std::uint64_t> Get_Public_Keys(const std::uint64_t a,     // a's private key
                                                                                        const std::uint64_t b,     // b's private key
                                                                                        const std::uint64_t p,     // modulo
                                                                                        std::uint64_t       gen = 0) // generator
{
    // Get generator of p if nothing is passed in.
    if (gen == 0)
    {
        gen = math::find_smallest_generator(p);
    }

    // Initialize GMP datatypes.
    mpz_class base(gen);
    mpz_class modulo(p);

    // Generate public keys of both A, B using their private keys: a, b.
    mpz_class PK_A{}; // Buffer for the Public key of A
    mpz_class PK_B{}; // Buffer for the Public key of B

    mpz_powm_ui(PK_A.get_mpz_t(),    // Result buffer
                base.get_mpz_t(),    // Base
                a,                   // Exponent
                modulo.get_mpz_t()); // Modulo

    mpz_powm_ui(PK_B.get_mpz_t(), 
                base.get_mpz_t(), 
                b, 
                modulo.get_mpz_t());

    return std::make_pair(std::make_pair(PK_A, PK_B), gen);
}

} // namespace utils
} // namespace crypto
#endif // CRYPTO_UTILS_HPP
