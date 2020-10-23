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
//! @params: ciphertext: numerical representation of the plaintext
//!          e:          small number coprime to phi(n)
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

} // namespace crypto
} // namespace algos
#endif // ALGORITHMS_HPP
