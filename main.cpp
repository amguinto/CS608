#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <math.h>
#include <set>
#include <string>
#include <vector>

// #include "utils/Math_utils.hpp"
#include "utils/Message_utils.hpp"

int main()
{
    auto a = 200;
    auto b = 100;

    std::string example = "njit";
    std::cout << "GCD = " << math::gcd(a, b) << std::endl;

    size_t n = 307;
    auto prime_factors = math::get_prime_factors_unq(n - 1);

    for (const auto& e : prime_factors)
    {
        std::cout << e << std::endl;
    }

    std::cout << "generator = " << math::find_smallest_generator(n) << std::endl;
}