#ifndef MENEZES_VANSTONE_HPP
#define MENEZES_VANSTONE_HPP

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
static inline std::pair<mpz_class, mpz_class> 
Menezes_Vanstone_EC(const mpz_class A_priv_key,
                    const mpz_class B_priv_key,
                    const std::pair<mpz_class, mpz_class> generator,
                    const mpz_class modulo,
                    const mpz_class a,
                    const mpz_class k,
                    const std::pair<mpz_class, mpz_class> message)
{
    assert(A_priv_key < modulo); 
    assert(B_priv_key < modulo);

    auto Pub_A = math::Scalar_mult_points_to_ECC(generator, A_priv_key, modulo, a);
    auto Pub_B = math::Scalar_mult_points_to_ECC(generator, B_priv_key, modulo, a);

    std::cout << "Pub_A = " << Pub_A.first << "\t" << Pub_A.second << std::endl;
    std::cout << "Pub_B = " << Pub_B.first << "\t" << Pub_B.second << std::endl;

    // A selects this secret key k < n
    // assert(k < A_priv_key);

    // A Creates "hint/clue"
    auto hint = math::Scalar_mult_points_to_ECC(generator, k, modulo, a);
    std::cout << "Hint = " << hint.first << "\t" << hint.second << std::endl;

    // Hide message m.
    auto mask = math::Scalar_mult_points_to_ECC(Pub_B, k, modulo, a);
    std::cout << "This is a point on the curve...Mask = " << mask.first << "\t" << mask.second << std::endl;

    mpz_class y_1;
    mpz_class base_y_1 = (mask.first * mask.second);
    std::cout << "Do: mask mod p" << std::endl;
    mpz_powm_ui(y_1.get_mpz_t(),
                base_y_1.get_mpz_t(), // Base
                1,                    // Exponent
                modulo.get_mpz_t());
    std::cout << "y_1 = " << y_1 << std::endl;

    mpz_class y_2;
    mpz_class base_y_2 = (message.first * message.second);
    std::cout << "Do: message coordinates mod p" << std::endl;
    mpz_powm_ui(y_2.get_mpz_t(),
                base_y_2.get_mpz_t(), // Base
                1,                    // Exponent
                modulo.get_mpz_t());
    std::cout << "y_2 = " << y_1 << std::endl;

    std::cout << "Send ciphertext: {hint, (y_1, y_2)}" << std::endl;
    std::cout << "{(" << hint.first << ", " << hint.second << "), (" << y_1 << ", " << y_2 << ")}" << std::endl;

    // Decryption
    std::cout << "Decryption:" << std::endl;
    std::cout << "Bob computes the new coords by doing : Bob_Priv_key * hint" << std::endl;

    auto decryption = math::Scalar_mult_points_to_ECC(hint, 8, modulo, a);
    std::cout << "Decryption = " << decryption.first << "\t" << decryption.second << std::endl;

    std::cout << "Now we can do the mult inverse equation given by using the decryption and y_1, and y_2" << std::endl;
    mpz_class mult_inverse_1;
    mpz_invert(mult_inverse_1.get_mpz_t(), decryption.first.get_mpz_t(), modulo.get_mpz_t());

    mpz_class mult_inverse_2;
    mpz_invert(mult_inverse_2.get_mpz_t(), decryption.second.get_mpz_t(), modulo.get_mpz_t());

    mpz_class plain_m1;
    mpz_class plain_m1_base = (y_1 * mult_inverse_1);
    mpz_powm_ui(plain_m1.get_mpz_t(), // buffer
                plain_m1_base.get_mpz_t(),  // base
                1,                   // exponent
                modulo.get_mpz_t()); // modulo}

    mpz_class plain_m2;
    mpz_class plain_m2_base = (y_2 * mult_inverse_2);
    mpz_powm_ui(plain_m2.get_mpz_t(), // buffer
                plain_m2_base.get_mpz_t(),  // base
                1,                   // exponent
                modulo.get_mpz_t()); // modulo}

    return std::make_pair(plain_m1, plain_m2);
}

} // namespace crypto
} // namespace algos
#endif // MENEZES_VANSTONE_HPP
