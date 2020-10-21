#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

// Standard C/C++
#include <algorithm>
#include <iostream>
#include <memory>

// Internal
#include "../utils/Math_utils.hpp"
#include "../utils/Message_utils.hpp"

namespace algos
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

//! @description: The purpose is to exchange a secret key over an insecure channel without transmitting the key over the channel.
//!               Compute the shared key, given the secret key and the modulus.
//! @params: a, b are the randomly chosen secret keys within the range of 0 < a,b < p -1
//!          p = modulo
static inline std::uint64_t Diffie_Hellman_Key_Exchange(const std::uint64_t a,
                                                        const std::uint64_t b,
                                                        const std::uint64_t p)
{
    std::uint64_t shared_key{};

    // Get Public Keys and generator.
    const auto PK_and_generator = algos::Get_Public_Keys(a, b, p);
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


//! @description: ElGamal Encryption to compute the ciphertext.
//! @params: a, b are the randomly chosen secret keys within the range of 0 < a,b < p -1
//!          p = modulo
//!          key (or k) is A's secret key
//!          plaintext = actual message we want to encrypt.
//! @return The ciphertext and the hint required to decrypt.
static inline std::pair<std::pair<mpz_class, mpz_class>, std::pair<std::uint64_t, std::uint64_t>> 
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
    const auto PK_and_generator = algos::Get_Public_Keys(a, b, p);
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
                PK_B->get_mpz_t(),    // Base
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
    mpz_powm_ui(hint.get_mpz_t(), // Result buffer
                base.get_mpz_t(),       // Base
                key,                    // Exponent
                modulo.get_mpz_t());    // Modulo

    return std::make_pair(std::make_pair(ciphertext, hint),
                          std::make_pair(p, std::move(Generator->get_ui())));
}

//! @description: ElGamal Encryption to compute the ciphertext.
//! @params: a, b are the randomly chosen secret keys within the range of 0 < a,b < p -1
//!          p = modulo
//!          key (or k) is A's secret key
//!          plaintext = actual message we want to encrypt.
//! @return The ciphertext and the hint required to decrypt.
static inline std::pair<std::pair<mpz_class, mpz_class>, std::pair<std::uint64_t, std::uint64_t>> 
ElGamal_Encryption(const std::uint64_t a,
                   const std::uint64_t b,
                   const std::uint64_t p,
                   const std::uint64_t key,
                   const std::uint64_t numeric_message)
{
    // Generate public keys of both A, B using their private keys: a, b.
    const auto PK_and_generator = algos::Get_Public_Keys(a, b, p);
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
                PK_B->get_mpz_t(),    // Base
                key,                 // Exponent
                modulo.get_mpz_t()); // Modulo

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

} // namespace algos
#endif // ALGORITHMS_HPP
