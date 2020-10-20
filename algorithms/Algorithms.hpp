#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

// Standard C/C++
#include <iostream>

// Internal
#include "../utils/Math_utils.hpp"

namespace algos
{
//! @description: The purpose is to exchange a secret key over an insecure channel without transmitting the key over the channel.
//!               Compute the shared key, given the secret key and the modulus.
//! @params: a, b are the randomly chosen secret keys within the range of 0 < a,b < p -1
//!          p = modulo
static inline std::uint64_t Diffie_Hellman_Key_Exchange(const std::uint64_t a, 
                                                        const std::uint64_t b, 
                                                        const std::uint64_t p)
{
    std::uint64_t shared_key{};

    // Get generator of p.
    auto generator = math::find_smallest_generator(p);

    // Initialize GMP datatypes.
    mpz_class base(generator);
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
    
    // Generate shared key using the public keys of each other.
    // PK_A is sent to B, and PK_B is sent to A.
    // We assume the generator and the modulus (p) are already known by both parties.
    mpz_class SK_A{}; // Buffer for the Private key of A
    mpz_class SK_B{}; // Buffer for the Private key of B

    mpz_powm_ui(SK_A.get_mpz_t(), 
                PK_B.get_mpz_t(), // Base is B's Public Key
                a,                // Exponent is A's own private key
                modulo.get_mpz_t());

    mpz_powm_ui(SK_B.get_mpz_t(), 
                PK_A.get_mpz_t(), // Base is A's Public Key
                b,                // Exponent is B's own private key
                modulo.get_mpz_t());
    
    // Make sure the shared key matches.
    if (SK_A == SK_B)
    {
        shared_key = mpz_get_ui(SK_A.get_mpz_t());
    }
    else
    {
        // Just for logging purposes without using a logging module.
        std::cout << "The shared keys were not computed correctly." << std::endl;
        std::cout << "Shared key of A = ";
        mpz_out_str(NULL, 10, SK_A.get_mpz_t());
        std::cout << std::endl;
        std::cout << "Shared key of B = ";
        mpz_out_str(NULL, 10, SK_B.get_mpz_t());
        std::cout << std::endl;
    }

    return shared_key;
}

} // namespace algos
#endif // ALGORITHMS_HPP
