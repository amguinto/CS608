#ifndef ELGAMAL_HPP
#define ELGAMAL_HPP

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
ElGamal_Encrypt(const std::uint64_t a,
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

    std::cout << "Public key of A (inside function) = " << *PK_A << std::endl;
    std::cout << "Public key of B (inside function) = " << *PK_B << std::endl;

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

    std::cout << "mask = " << mask << std::endl;
    assert(mask != 1);

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
    mpz_class ciphertext_base = mask * compressed_num_plaintext;
    mpz_powm_ui(ciphertext.get_mpz_t(),      // Result buffer
                ciphertext_base.get_mpz_t(), // Base
                1,                           // Exponent
                modulo.get_mpz_t());         // Modulo

    std::cout << "ciphertext = " << ciphertext << std::endl;

    // Calculate Hint.
    mpz_class hint{};
    mpz_powm_ui(hint.get_mpz_t(),    // Result buffer
                base.get_mpz_t(),    // Base
                key,                 // Exponent
                modulo.get_mpz_t()); // Modulo

    std::cout << "hint = " << hint << std::endl;

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
ElGamal_Encrypt(const std::uint64_t a,
                const std::uint64_t b,
                const std::uint64_t p,
                const std::uint64_t key,
                const std::uint64_t numeric_message)
{
    // VERY IMPORTANT
    // assert(numeric_message < p);

    // Generate public keys of both A, B using their private keys: a, b.
    const auto PK_and_generator = crypto::utils::Get_Public_Keys(a, b, p);
    const auto PK_A      = std::make_unique<mpz_class>(PK_and_generator.first.first);
    const auto PK_B      = std::make_unique<mpz_class>(PK_and_generator.first.second);
    const auto Generator = std::make_unique<mpz_class>(PK_and_generator.second);

    std::cout << "Public key of A (inside function) = " << *PK_A << std::endl;
    std::cout << "Public key of B (inside function) = " << *PK_B << std::endl;

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

    std::cout << "mask = " << mask << std::endl;
    assert(mask != 1);

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
    
    std::cout << "ciphertext = " << ciphertext << std::endl;

    // Calculate Hint.
    mpz_class hint{};
    mpz_powm_ui(hint.get_mpz_t(),    // Result buffer
                base.get_mpz_t(),    // Base
                key,                 // Exponent
                modulo.get_mpz_t()); // Modulo

    std::cout << "hint = " << hint << std::endl;

    return std::make_pair(std::make_pair(ciphertext, hint),
                          std::make_pair(p, std::move(Generator->get_ui())));
}

//! @description: ElGamal Decryption to compute the plaintext.
//! @params: a, b:   the randomly chosen secret keys that were used for encryption
//!          params: {Ciphertext, Hint}, {Modulo, Generator}
//! @return The ciphertext and the hint required to decrypt.
static inline mpz_class 
ElGamal_Decrypt(const std::uint64_t a,
                const std::uint64_t b,
                const std::pair<std::pair<mpz_class, mpz_class>,
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
    std::cout << "q = " << q << std::endl;

    // Get multiplicate inverse R.
    mpz_class R{};
    mpz_powm_ui(R.get_mpz_t(),                   // Result buffer
                params.first.second.get_mpz_t(), // Base (Hint)
                q,                               // Exponent
                modulo.get_mpz_t());             // Modulo

    std::cout << "R = " <<  R << std::endl;

    // Get the decrypted numerical representation
    mpz_class decryption{};

    // Decryption: Ciphertext * R mod p
    const mpz_class decrypt_base = params.first.first * R;
    mpz_powm_ui(decryption.get_mpz_t(),   // Result buffer
                decrypt_base.get_mpz_t(), // Base
                1,                        // Exponent
                modulo.get_mpz_t());      // Modulo

    std::cout << "decryption (inside function) = " <<  decryption << std::endl;

    return decryption;
}

//! @description: ElGamal Public Key Generation for the sender (Digital Signatures)
//! @params: secret_key: r in the equation, a random integer such that 0 < r < p - 1
//!          modulo
//! @return {key}, {generator, modulo}
static inline std::pair<mpz_class, std::pair<mpz_class, mpz_class>> 
ElGamal_Key_Generation(const mpz_class& secret_key,
                       const mpz_class& modulo)
{
    assert(secret_key > 0 && secret_key < modulo - 1);

    mpz_class key{};
    mpz_class generator(math::find_smallest_generator(modulo));
    mpz_powm_ui(key.get_mpz_t(),       // Result buffer
                generator.get_mpz_t(), // Base
                secret_key.get_ui(),   // Exponent
                modulo.get_mpz_t());   // Modulo

    return std::make_pair(key, std::make_pair(generator, modulo));
}

//! @description: ElGamal Signing to sign message M
//! @params: R: different secret key from key generation, 0 < R < modulo - 1 and coprime to modulo - 1
//!          r: secret key used for key generation
//!          message:
//!          modulo:
//!          generator
//! @return {message, Public Key}, {X, Y}}
static inline std::pair<std::pair<mpz_class, mpz_class>, std::pair<mpz_class, mpz_class>> 
ElGamal_Sign(const mpz_class& R, // Different Secret Key
             const mpz_class& r,
             const mpz_class& message,
             const mpz_class& modulo,
             const mpz_class& generator)
{
    // VERY IMPORTANT.
    assert(math::is_coprime(R, mpz_class(modulo - 1)));

    // Find X = g^R mod p
    mpz_class X{};
    mpz_powm_ui(X.get_mpz_t(),         // Result buffer
                generator.get_mpz_t(), // Base
                R.get_ui(),            // Exponent
                modulo.get_mpz_t());   // Modulo


    // Find Y, so message = rX + RY mod modulo-1
    // Rewritten as Y = (M - rX) * R^-1 modulo-1
    mpz_class Y{};
    mpz_class R_inverse(math::Multiplicative_Inverse(R, modulo - 1));
    mpz_class base((message - (r * X)) * R_inverse);
    mpz_class p(modulo - 1);
    mpz_powm_ui(Y.get_mpz_t(),    // Result buffer
                base.get_mpz_t(), // Base
                1,                // Exponent
                p.get_mpz_t());   // Modulo
    
    // Calculate Public Key K to be used for verification.
    // g^r
    mpz_class K{};
    mpz_powm_ui(K.get_mpz_t(),        // Result buffer
               generator.get_mpz_t(), // Base
               r.get_ui(),            // Exponent
               modulo.get_mpz_t());   // Modulo

    return std::make_pair(std::make_pair(message, K),
                          std::make_pair(X, Y));
}

//! @description: ElGamal Verification to verify message M
//! @params: public_key: public key of the sender
//!          X: param1
//!          Y: param2
//!          modulo:
//!          message:
//!          generator
//! @return whether or not the calculations add up to be correct. The actual number is irrelevant.
static inline bool 
ElGamal_Verify(const mpz_class& public_key, // public key of the sender
               const mpz_class& X,
               const mpz_class& Y,
               const mpz_class& message,
               const mpz_class& modulo,
               const mpz_class& generator)
{
    // We want to solve (K^X)(K^Y) mod p
    mpz_class K_X{};
    mpz_pow_ui(K_X.get_mpz_t(),        // Result buffer
               public_key.get_mpz_t(), // Base
               X.get_ui());            // Exponent
    // std::cout << "K^X = " << K_X << std::endl;

    // X^Y
    mpz_class X_Y{};
    mpz_pow_ui(X_Y.get_mpz_t(), // Result buffer
               X.get_mpz_t(),   // Base
               Y.get_ui());     // Exponent

    // std::cout << "X^Y = " << X_Y << std::endl;

    mpz_class base(K_X * X_Y);
    mpz_class result{};
    mpz_powm_ui(result.get_mpz_t(),  // Result buffer
                base.get_mpz_t(),    // Base
                1,                   // Exponent
                modulo.get_mpz_t()); // Modulo

    std::cout << "ElGamal_Verify, A = " << result << std::endl;

    // Calculate verification
    mpz_class verification{};
    mpz_powm_ui(verification.get_mpz_t(), // Result buffer
                generator.get_mpz_t(),    // Base
                message.get_ui(),         // Exponent
                modulo.get_mpz_t());      // Modulo


    std::cout << "Verification g^m mod p = " << verification << std::endl;

    return result == verification;
}

} // namespace crypto
} // namespace algos
#endif // ELGAMAL_HPP
