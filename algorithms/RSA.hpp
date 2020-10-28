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
//! @description: Create RSA Keys. Used for both Cryptosystem and Digital Signature
//! @params: p, q: random large primes
//!          e:    small number coprime to phi(n)
//! @return: {m, e}, (Public Keys)
//!           d      (Private Key, so in the real world, this is kept secret)
static inline std::pair<std::pair<mpz_class, mpz_class>, mpz_class>
RSA_Key_Generation_DS(const mpz_class p,
                      const mpz_class q,
                      const mpz_class e)
{
    mpz_class P(p);
    mpz_class Q(q);
    mpz_class m = P * Q;
    mpz_class phi_m = math::Euler_Totient_primes(P, Q);

    // Make sure that the small e is coprime to phi_n
    assert(math::is_coprime(phi_m, e));

    // Find d such that d * e mod phi_m = 1;
    // We can rewrite the function so that its: d mod phi_m = 1 * e^-1 (or the inverse of e)
    mpz_class d{};
    mpz_invert(d.get_mpz_t(),
               mpz_class(e).get_mpz_t(), // What we want the inverse of
               phi_m.get_mpz_t());       // Modulo

    // Test.
    mpz_class check{};
    mpz_class base = e * d;
    mpz_powm_ui(check.get_mpz_t(),  // Result buffer
                base.get_mpz_t(),   // Base (d * e)
                1,                  // Exponent
                phi_m.get_mpz_t()); // Modulo)

    assert(check == 1);

    return std::make_pair(std::make_pair(m, e), d);
}

//! @description: Create RSA Keys. Used for both Cryptosystem and Digital Signature
//! @params: p, q: random large primes
//!          e:    small number coprime to phi(n)
//! @return: {e, n}, (Public Keys)
//!           d      (Private Key, so in the real world, this is kept secret)
static inline std::pair<std::pair<mpz_class, mpz_class>, mpz_class>
RSA_Key_Generation(const mpz_class p,
                   const mpz_class q,
                   const mpz_class e)
{
    mpz_class P(p);
    mpz_class Q(q);
    mpz_class n = P * Q;
    mpz_class phi_n = math::Euler_Totient_primes(P, Q);

    // Make sure that the small e is coprime to phi_n
    assert(math::is_coprime(phi_n, e));

    std::cout << "phi_n (in function) = " << phi_n << std::endl; 
    // Find d such that d * e mod phi_n = 1;
    // We can rewrite the function so that its: d mod phi_n = 1 * e^-1 (or the inverse of e)
    mpz_class d{};
    mpz_invert(d.get_mpz_t(),
               mpz_class(e).get_mpz_t(), // What we want the inverse of
               phi_n.get_mpz_t());       // Modulo

    std::cout << "d, also known as the private key (in function) = " << d << std::endl; 

    // Test.
    mpz_class check{};
    mpz_class base = e * d;
    mpz_powm_ui(check.get_mpz_t(),  // Result buffer
                base.get_mpz_t(),   // Base (d * e)
                1,                  // Exponent
                phi_n.get_mpz_t()); // Modulo)

    assert(check == 1);

    return std::make_pair(std::make_pair(e, n), d);
}

//! @description: Encrypt using the RSA protocol
//! @params: message:   numerical representation of the plaintext
//!          e:         small number coprime to phi(n)
//!          n:         n = P * Q
//! @return ciphertext: return the numerical representation of the ciphertext
static inline mpz_class
RSA_Encrypt(const mpz_class& message,
            const mpz_class& e,
            const mpz_class& n)
{
    // VERY IMPORTANT
    assert(n > message);

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
//!          sender_semiprime: m, from m = p * q (provided to us)
//!          receiver_semiprime: n, from n = r * s (provided to us)
//!          PK_receiver: public key pair of the receiver {n, h}
//!          message: the compressed, numeric form of the message
//! @return  encryption: the signed encrypted message.
static inline mpz_class
RSA_Sign_and_Encrypt(const mpz_class& private_key,
                     const mpz_class& sender_semiprime,   // m = p * q
                     const mpz_class& receiver_semiprime, // n = r * s
                     const std::pair<mpz_class, mpz_class>& PK_receiver, // public key of receiver
                     const mpz_class& message)
{
    // VERY IMPORTANT
    assert(sender_semiprime > message);
    // std::cout << "sender_semiprime = " << sender_semiprime << "\tmessage = " << message << std::endl;
    mpz_class signature{};
    mpz_powm_ui(signature.get_mpz_t(),
                message.get_mpz_t(),           // Base is the message
                private_key.get_ui(),          // Exponent is the private key
                sender_semiprime.get_mpz_t()); // Modulo is m, from m = p * q

    std::cout << "x = " << signature << "\t";
    mpz_class encryption{};
    mpz_powm_ui(encryption.get_mpz_t(),
                signature.get_mpz_t(),           // Base is the signature from the previous calculation
                PK_receiver.second.get_ui(),     // Exponent is the second part (h) of the public key of the Receiver
                receiver_semiprime.get_mpz_t()); // Modulo is n, from n = r * s

    std::cout << "y = " << encryption << std::endl;
    return encryption;
}

//! @description: Sign and Encrypt using RSA Digital Signatures
//! @params: private_key: private key of the receiver
//!          sender_semiprime: m, from m = p * q (provided to us)
//!          receiver_semiprime: n, from n = r * s (provided to us)
//!          PK_sender: public key pair of the sender {m, e}
//!          signed_message: the signed message from the sender
//! @return  verified_message
static inline mpz_class
RSA_Decrypt_and_Verify(const mpz_class& private_key,
                       const mpz_class& sender_semiprime,   // m = p * q
                       const mpz_class& receiver_semiprime, // n = r * s
                       const std::pair<mpz_class, mpz_class>& PK_sender, // public key of the sender
                       const mpz_class& signed_message)

{
    mpz_class decryption{};
    mpz_powm_ui(decryption.get_mpz_t(),
                signed_message.get_mpz_t(),      // Base is the encryption from the Sender
                private_key.get_ui(),            // Exponent is the private key of the receiver
                receiver_semiprime.get_mpz_t()); // Modulo is n, from n = r * s

    std::cout << "z = " << decryption << "\t";

    mpz_class verification{};
    mpz_powm_ui(verification.get_mpz_t(),
                decryption.get_mpz_t(),        // Base is the signature from the previous calculation
                PK_sender.second.get_ui(),     // Exponent is the second part of the public key of the Sender
                sender_semiprime.get_mpz_t()); // Modulo is m, from m = p * q
    
    std::cout << "u = " << verification << std::endl;

    // Make sure we verify the message.
    return verification;
}

} // namespace crypto
} // namespace algos
#endif // ALGORITHMS_HPP
