#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <math.h>
#include <set>
#include <string>
#include <vector>

#include "Math_utils.hpp"
#include "Message_utils.hpp"

int main()
{
    auto a = 200;
    auto b = 100;

    std::string example = "njit";
    std::cout << "GCD = " << math::gcd(a, b) << std::endl;

    auto naive = message::naive_plaintext_numeric(example);

    std::cout << "Numerical Representation = ";
    for (const auto& e : naive)
    {
        std::cout << e << "\t";
    }
    std::cout << std::endl;

    auto compressed = message::encode_naive_representation(naive);
    std::cout << "Encoded compressed Representation = " << compressed << std::endl;

    std::string decoded = message::decode_naive_representation(compressed);
    std::cout << decoded << std::endl;
    std::cout << std::endl;

    size_t n = 307;
    auto prime_factors = math::get_prime_factors_unq(n - 1);

    for (const auto& e : prime_factors)
    {
        std::cout << e << std::endl;
    }

    std::cout << "generator = " << math::find_generator(n) << std::endl;
}