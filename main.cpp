#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <math.h>
#include <set>
#include <string>
#include <vector>

#include "utils/Math_utils.hpp"
#include "utils/Message_utils.hpp"
#include "algorithms/Diffie_Hellman_KE.hpp"
#include "algorithms/ElGamal.hpp"


int main()
{
    // auto p = 104801; // modulo
    // auto r = 7;
    // auto R = 9;
    // auto message = 17;
    /************GENERATOR*******************
    **************************************/
    // auto gen = math::find_smallest_generator(p);
    // std::cout << "generator for " << p << " = " << gen << std::endl;

    /************MODULAR*******************
    **************************************/
    // mpz_class result{};
    // mpz_class base(gen);
    // mpz_class modulo(p);

    // mpz_powm_ui(result.get_mpz_t(),  // Result buffer
    //             base.get_mpz_t(),    // Base
    //             R,                   // Exponent
    //             modulo.get_mpz_t()); // Modulo)

    // std::cout << "result = " << result << std::endl;

    /************MODULAR INVERSE***********
    **************************************/
    // mpz_class result_inv{};
    // mpz_class base_inv(5);
    // mpz_class modulo_inv(5);
    // mpz_invert(result_inv.get_mpz_t(),
    //            base_inv.get_mpz_t(),     // What we want the inverse of
    //            modulo_inv.get_mpz_t());  // Modulo


    // std::cout << "Square and multiply exponentiation:" << std::endl;
    // mpz_class modulo(29);
    // mpz_class base(16);
    // mpz_class exponent(2);
    // std::cout << math::Square_and_Multiply_Exponentiation(modulo, base, exponent) << std::endl;
}