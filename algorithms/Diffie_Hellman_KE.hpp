#ifndef DIFFIE_HELLMAN_KE_HPP
#define DIFFIE_HELLMAN_KE_HPP

// Standard C/C++
#include <cassert>
#include <memory>

// Internal
#include "../utils/Math_utils.hpp"
#include "../utils/Crypto_utils.hpp"

namespace crypto
{
namespace algos
{
//! @description: The purpose is to exchange a secret key over an insecure channel without transmitting the key over the channel.
//!               Compute the shared key, given the secret key and the modulus.
//! @params:      a, b are the randomly chosen secret keys within the range of 0 < a,b < p -1
//!               p = modulo
static inline std::uint64_t 
Diffie_Hellman_Key_Exchange(const std::uint64_t a,
                            const std::uint64_t b,
                            const std::uint64_t p)
{
    std::uint64_t shared_key{};

    // Get Public Keys and generator.
    const auto PK_and_generator = crypto::utils::Get_Public_Keys(a, b, p);
    const auto PK_A      = std::make_unique<mpz_class>(PK_and_generator.first.first);
    const auto PK_B      = std::make_unique<mpz_class>(PK_and_generator.first.second);
    const auto Generator = std::make_unique<mpz_class>(PK_and_generator.second);

    // Initialize GMP datatypes.
    mpz_class base(*Generator); // generator
    mpz_class modulo(p);

    // Generate shared key using the public keys of each other.
    // PK_A is sent to B, and PK_B is sent to A.
    // We assume the generator and the modulus (p) are already known by both parties.
    mpz_class SK_A{}; // Buffer for the Private key of A
    mpz_class SK_B{}; // Buffer for the Private key of B

    mpz_powm_ui(SK_A.get_mpz_t(),
                PK_B->get_mpz_t(),   // Base is B's Public Key
                a,                   // Exponent is A's own private key
                modulo.get_mpz_t());

    mpz_powm_ui(SK_B.get_mpz_t(),
                PK_A->get_mpz_t(),   // Base is A's Public Key
                b,                   // Exponent is B's own private key
                modulo.get_mpz_t());

    // Make sure the shared key matches.
    assert(SK_A == SK_B);

    return mpz_get_ui(SK_A.get_mpz_t());
}

} // namespace crypto
} // namespace algos
#endif // DIFFIE_HELLMAN_KE_HPP
