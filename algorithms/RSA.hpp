#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

// Standard C/C++
#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>

// Internal
#include "../utils/Math_utils.hpp"
#include "../utils/Crypto_utils.hpp"

namespace crypto
{
namespace algos
{
//! @description: Create RSA Keys
//! @params: p, q: random large primes
//!          e:    small number coprime to phi(n)
//! @return: {e, n}, (Public Keys)
//!           d      (Private Key, so in the real world, this is kept secret)
static inline std::pair<std::pair<std::uint64_t, mpz_class>, mpz_class>
RSA_Key_Generation(const std::uint64_t p,
                   const std::uint64_t q,
                   const std::uint64_t e)
{
    mpz_class P(p);
    mpz_class Q(q);
    mpz_class n = P * Q;
    mpz_class phi_n = math::Euler_Totient_primes(P, Q);


    // Find d such that d * e mod phi_n = 1;
    // We can rewrite the function so that its: e mod phi_n = 1 * e^-1 (or the inverse of e)
    mpz_class d{};
    mpz_invert(d.get_mpz_t(),
               mpz_class(e).get_mpz_t(), // What we want the inverse of
               phi_n.get_mpz_t());       // Modulo

    return std::make_pair(std::make_pair(e, n), d);
}

//! @description: Encrypt using the RSA protocol
//! @params: message:   numerical representation of the plaintext
//!          e:         small number coprime to phi(n)
//! @return ciphertext: return the numerical representation of the ciphertext
static inline mpz_class
RSA_Encrypt(const mpz_class& message,
            const mpz_class& e,
            const mpz_class& n)
{
    mpz_class ciphertext{};
    mpz_powm_ui(ciphertext.get_mpz_t(),
                mpz_class(message).get_mpz_t(), // Base is the message
                e.get_ui(),                     // Exponent is small prime e, coprime to phi(n)
                n.get_mpz_t());                 // Modulo is n
    
    return ciphertext;
}

//! @description: Decrypt using the RSA protocol
//! @params: d: private key
//!          n: modulo
//! @return  message:    return the numerical representation of the plaintext
static inline mpz_class
RSA_Decrypt(const mpz_class& ciphertext,
            const mpz_class& d,
            const mpz_class& n)
{
    mpz_class message{};
    mpz_powm_ui(message.get_mpz_t(),
                ciphertext.get_mpz_t(), // Base is the message
                d.get_ui(),             // Exponent is the private key
                n.get_mpz_t());         // Modulo is n
    
    return message;
}

//! @description: Sign and Encrypt using RSA Digital Signatures
//! @params: private_key: private key of the sender
//!          PK_sender: public key pair of the sender {m, e}
//!          PK_receiver: public key pair of the receiver {m, e}
//! @return  encryption
static inline mpz_class
RSA_Sign_and_Encrypt(const mpz_class& private_key,
                     const std::pair<mpz_class, mpz_class>& PK_sender,   // {m, e}
                     const std::pair<mpz_class, mpz_class>& PK_receiver, // {n, h}
                     const mpz_class& message)
{
    mpz_class signature{};
    mpz_powm_ui(signature.get_mpz_t(),
                message.get_mpz_t(),          // Base is the message
                private_key.get_ui(),         // Exponent is the private key
                PK_sender.first.get_mpz_t()); // Modulo is first part of the public key of the Sender

    mpz_class encryption{};
    mpz_powm_ui(encryption.get_mpz_t(),
                signature.get_mpz_t(),          // Base is the signature from the previous calculation
                PK_receiver.second.get_ui(),    // Exponent is the second part of the public key of the Receiver
                PK_receiver.first.get_mpz_t()); // Modulo is first part of the public key of the Receiver

    return encryption;
}

//! @description: Sign and Encrypt using RSA Digital Signatures
//! @params: private_key: private key of the receiver
//!          encryption:
//!          PK_sender: public key pair of the sender {m, e}
//!          PK_receiver: public key pair of the receiver {m, e}
//! @return  encryption
static inline mpz_class
RSA_Decrypt_and_Verify(const mpz_class& private_key,
                       const mpz_class& encryption,
                       const std::pair<mpz_class, mpz_class>& PK_sender,   // {m, e}
                       const std::pair<mpz_class, mpz_class>& PK_receiver, // {n, h}
                       const mpz_class& message)
{
    mpz_class decryption{};
    mpz_powm_ui(decryption.get_mpz_t(),
                encryption.get_mpz_t(),         // Base is the encryption from the Sender
                private_key.get_ui(),           // Exponent is the private key of the receiver
                PK_receiver.first.get_mpz_t()); // Modulo is first part of the public key of the Receiver.

    mpz_class verification{};
    mpz_powm_ui(verification.get_mpz_t(),
                decryption.get_mpz_t(),        // Base is the signature from the previous calculation
                PK_sender.second.get_ui(),    // Exponent is the second part of the public key of the Sender
                PK_sender.first.get_mpz_t()); // Modulo is first part of the public key of the Sender

    // Make sure we verify the message.
    return verification == message;
}

} // namespace crypto
} // namespace algos
#endif // ALGORITHMS_HPP
