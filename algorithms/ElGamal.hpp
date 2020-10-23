#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

// Standard C/C++
#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>

// Internal
#include "../utils/Message_utils.hpp"
#include "../utils/Crypto_utils.hpp"

namespace crypto
{
namespace algos
{
//! @description: ElGamal Encryption to compute the ciphertext.
//! @params: a, b are the randomly chosen secret keys within the range of 0 < a,b < p -1
//!          p = modulo
//!          key (or k) is A's secret key
//!          plaintext = actual message we want to encrypt.
//! @return The ciphertext and the hint required to decrypt.
static inline std::pair<std::pair<mpz_class, mpz_class>, 
                       std::pair<std::uint64_t, std::uint64_t>> 
ElGamal_Encryption(const std::uint64_t a,
                   const std::uint64_t b,
                   const std::uint64_t p,
                   const std::uint64_t key,
                   const std::string   plaintext)
{
    // Get numerical representation of the plaintext.
    auto num_plaintext = message::naive_plaintext_numeric(plaintext);

    // Compress ciphertext
    auto compressed_num_plaintext = message::encode_naive_representation(num_plaintext);

    // Generate public keys of both A, B using their private keys: a, b.
    const auto PK_and_generator = crypto::utils::Get_Public_Keys(a, b, p);
    const auto PK_A      = std::make_unique<mpz_class>(PK_and_generator.first.first);
    const auto PK_B      = std::make_unique<mpz_class>(PK_and_generator.first.second);
    const auto Generator = std::make_unique<mpz_class>(PK_and_generator.second);

    // Initialize GMP datatypes.
    mpz_class base(*Generator); // generator
    mpz_class modulo(p);

    // Calculate Mask.
    // A uses B's public key to encrypt. Therefore it is used in the mask calculation.
    // A also uses its secret key (exponent) in the mask calculation
    mpz_class mask{};
    mpz_powm_ui(mask.get_mpz_t(),    // Result buffer
                PK_B->get_mpz_t(),   // Base
                key,                 // Exponent
                modulo.get_mpz_t()); // Modulo

    // Calculate Ciphertext.
    // A uses B's public key to encrypt. Therefore it is used in the mask calculation.
    // A also uses its secret key (exponent) in the mask calculation
    mpz_class ciphertext{};
    mpz_class ciphertext_base = mask * compressed_num_plaintext;
    mpz_powm_ui(ciphertext.get_mpz_t(),      // Result buffer
                ciphertext_base.get_mpz_t(), // Base
                1,                           // Exponent
                modulo.get_mpz_t());         // Modulo

    
    // Calculate Hint.
    mpz_class hint{};
    mpz_powm_ui(hint.get_mpz_t(),    // Result buffer
                base.get_mpz_t(),    // Base
                key,                 // Exponent
                modulo.get_mpz_t()); // Modulo

    return std::make_pair(std::make_pair(ciphertext, hint),
                          std::make_pair(p, std::move(Generator->get_ui())));
}

//! @description: ElGamal Encryption to compute the ciphertext.
//! @params: a, b are the randomly chosen secret keys within the range of 0 < a,b < p -1
//!          p = modulo
//!          key (or k) is A's secret key
//!          plaintext = actual message we want to encrypt.
//! @return The ciphertext and the hint required to decrypt.
static inline std::pair<std::pair<mpz_class, mpz_class>, 
                        std::pair<std::uint64_t, std::uint64_t>> 
ElGamal_Encryption(const std::uint64_t a,
                   const std::uint64_t b,
                   const std::uint64_t p,
                   const std::uint64_t key,
                   const std::uint64_t numeric_message)
{
    // Generate public keys of both A, B using their private keys: a, b.
    const auto PK_and_generator = crypto::utils::Get_Public_Keys(a, b, p);
    const auto PK_A      = std::make_unique<mpz_class>(PK_and_generator.first.first);
    const auto PK_B      = std::make_unique<mpz_class>(PK_and_generator.first.second);
    const auto Generator = std::make_unique<mpz_class>(PK_and_generator.second);

    // Initialize GMP datatypes.
    mpz_class base(*Generator); // generator
    mpz_class modulo(p);

    // Calculate Mask.
    // A uses B's public key to encrypt. Therefore it is used in the mask calculation.
    // A also uses its secret key (exponent) in the mask calculation
    mpz_class mask(1); // Initialize to 1 so we can find a better mask.
    mpz_powm_ui(mask.get_mpz_t(),    // Result buffer
                PK_B->get_mpz_t(),   // Base
                key,                 // Exponent
                modulo.get_mpz_t()); // Modulo

    // We need to make sure the mask is NEVER 1.
    // The inverse would be too trivial to calculate.
    if (mask == 1)
    {
        return std::make_pair(std::make_pair(mpz_class{}, mpz_class{}),
                              std::make_pair(p, std::move(Generator->get_ui())));
    }

    // Calculate Ciphertext.
    // A uses B's public key to encrypt. Therefore it is used in the mask calculation.
    // A also uses its secret key (exponent) in the mask calculation
    mpz_class ciphertext{};
    const mpz_class ciphertext_base = mask * numeric_message;
    mpz_powm_ui(ciphertext.get_mpz_t(),      // Result buffer
                ciphertext_base.get_mpz_t(), // Base
                1,                           // Exponent
                modulo.get_mpz_t());         // Modulo
    
    
    // Calculate Hint.
    mpz_class hint{};
    mpz_powm_ui(hint.get_mpz_t(),    // Result buffer
                base.get_mpz_t(),    // Base
                key,                 // Exponent
                modulo.get_mpz_t()); // Modulo

    return std::make_pair(std::make_pair(ciphertext, hint),
                          std::make_pair(p, std::move(Generator->get_ui())));
}

static inline mpz_class ElGamal_Decryption(const std::uint64_t a,
                                           const std::uint64_t b,
                                           const std::pair<
                                                    std::pair<mpz_class,     mpz_class>,
                                                    std::pair<std::uint64_t, std::uint64_t>>& params)
{
    // Make sure we actually have the ciphertext and hint.
    // Otherwise, we can't decrypt.
    if (params.first.first.get_ui()  == 0 &&
        params.first.second.get_ui() == 0)
    {
        return mpz_class{};
    }

    // Use the same Modulo and Generator that was used for encryption.
    // Initialize GMP datatypes.
    mpz_class modulo(params.second.first);
    mpz_class generator(params.second.second);

    // Find q: p - 1 - b.
    const auto q = params.second.first - 1 - b;
    
    // Get multiplicate inverse R.
    mpz_class R{};
    mpz_powm_ui(R.get_mpz_t(),                   // Result buffer
                params.first.second.get_mpz_t(), // Base (Hint)
                q,                               // Exponent
                modulo.get_mpz_t());             // Modulo

    // Get the decrypted numerical representation
    mpz_class decryption{};

    // Decryption: Ciphertext * R mod p
    const mpz_class decrypt_base = params.first.first * R;
    mpz_powm_ui(decryption.get_mpz_t(),   // Result buffer
                decrypt_base.get_mpz_t(), // Base
                1,                        // Exponent
                modulo.get_mpz_t());      // Modulo

    return decryption;
}

} // namespace crypto
} // namespace algos
#endif // ALGORITHMS_HPP
