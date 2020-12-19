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
    mpz_class SK_A{}; // Buffer for the Public key of A
    mpz_class SK_B{}; // Buffer for the Public key of B

    mpz_powm_ui(SK_A.get_mpz_t(),
                PK_B->get_mpz_t(),   // Base is B's Public Key
                a,                   // Exponent is A's own private key
                modulo.get_mpz_t());

    std::cout << "Public key of A (g^a)= " << *PK_A << std::endl;

    mpz_powm_ui(SK_B.get_mpz_t(),
                PK_A->get_mpz_t(),   // Base is A's Public Key
                b,                   // Exponent is B's own private key
                modulo.get_mpz_t());

    std::cout << "Public key of B (g^b)= " << *PK_B << std::endl;

    // Make sure the shared key matches.
    assert(SK_A == SK_B);

    return mpz_get_ui(SK_A.get_mpz_t());
}

static inline std::pair<mpz_class, mpz_class> 
Diffie_Hellman_Key_Exchange_EC(const mpz_class A_priv_key,
                               const mpz_class B_priv_key,
                               const std::pair<mpz_class, mpz_class> generator,
                               const mpz_class modulo,
                               const mpz_class a)
{
    // X_a = Alice's Private Key
    // X_b = Bob's Private Key
    // Calculate Public Keys
    auto Pub_A = math::Scalar_mult_points_to_ECC(generator, A_priv_key, modulo, a);
    auto Pub_B = math::Scalar_mult_points_to_ECC(generator, B_priv_key, modulo, a);

    std::cout << "Pub_A = " << Pub_A.first << "\t" << Pub_A.second << std::endl;
    std::cout << "Pub_B = " << Pub_B.first << "\t" << Pub_B.second << std::endl;
    // Now we can calculate the shared key
    auto SK_A = math::Scalar_mult_points_to_ECC(Pub_B, A_priv_key, modulo, a);
    auto SK_B = math::Scalar_mult_points_to_ECC(Pub_A, B_priv_key, modulo, a);

    std::cout << "SK_A = " << SK_A.first << "\t" << SK_A.second << std::endl;
    std::cout << "SK_B = " << SK_B.first << "\t" << SK_B.second << std::endl;

    if (SK_A.first != SK_B.first && SK_A.second != SK_B.second)
    {
        std::cout << "ERROR!" << std::endl;
        std::cout << "SK_A = (" << SK_A.first << ", " << SK_A.second 
                  << ")\tSK_B = (" << SK_B.first << ", " << SK_B.second << ")" << std::endl;
    }

    return SK_A;
}

} // namespace crypto
} // namespace algos
#endif // DIFFIE_HELLMAN_KE_HPP
