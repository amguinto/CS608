#include <algorithm>
#include <limits>
#include <math.h>
#include <set>
#include <vector>

// GMP
#include <gmpxx.h>

namespace math
{
// Basic Eucliean Algorithm
static inline std::uint64_t gcd(const size_t a, const size_t b)
{
    if (a == 0)
    {
        return b;
    }
    return gcd(b % a, a);
}

// Get all prime factors
// Implicit conversion takes care of smaller integer types.
// Not efficient, but there's not enough type to add template metaprogramming.
static inline std::vector<std::uint64_t> get_prime_factors(std::uint64_t num)
{
    std::vector<size_t> prime_factors;
    prime_factors.reserve(5);

    while(num % 2 == 0)
    {
        prime_factors.emplace_back(2);
        num /= 2;
    }

    // num = odd at this point
    for (int i = 3; i <= std::sqrt(num); i+= 2)
    {
        while (num % i == 0)
        {
            prime_factors.emplace_back(i);
            num /= i;
        }
    }

    // N is a prime greater than 2.
    if (num > 2)
    {
        prime_factors.emplace_back(num);
    }

    return prime_factors;
}

// Get only the unique prime factors
static inline std::set<size_t> get_prime_factors_unq(const std::uint64_t num)
{
    auto all_primes = get_prime_factors(num);
    std::set<size_t> prime_factors_unq(all_primes.begin(), all_primes.end()); 

    return prime_factors_unq;
}


// Helper function since std::pow isn't overloaded for integer types.
template<typename T>
static inline mpz_class IPow(const T base, const T power)
{
    mpz_class rop{};
    mpz_ui_pow_ui(rop.get_mpz_t(), base, power);
    return rop;
}

static inline std::size_t find_smallest_generator(const size_t num)
{
    // Get factors of p - 1
    auto factors = get_prime_factors_unq(num - 1);

    auto generator = 0;
    bool found = false;
    
    // Initialize modulo.
    mpz_class modulo(num);

    // try a g, such that 1 < g < p - 1
    for (std::size_t i = 2; i < num - 1; ++i)
    {
        if (found)
        {
            break;
        }

        // Initialize new generator.
        mpz_class base(i);

        for (const auto& e : factors)
        {
            mpz_class result{};
            std::size_t pow = (num - 1) / e;
            mpz_powm_ui(result.get_mpz_t(), 
                        base.get_mpz_t(), 
                        pow, 
                        modulo.get_mpz_t());

            // std::cout << "result = " << result << "\ti = " << i << "\texponent = " << pow << std::endl;
            if (result == 1)
            {
                generator = 0;
                found = false;
                // std::cout << i << " is not a generator." << std::endl;
                break;
            }
            else
            {
                generator = i;
                found = true;
            }
            
        }
    }
    return generator;
}

// @params: num = modulo
//          g = generator to try
static inline bool is_generator(const size_t num, const size_t g)
{
     // Get factors of p - 1
    auto factors = get_prime_factors_unq(num - 1);
    bool gtor = true;

    // Initialize new generator.
    mpz_class base(g);

    // Initialize modulo.
    mpz_class modulo(num);

    for (const auto& e : factors)
    {
        mpz_class result{};
        std::size_t pow = (num - 1) / e;
        mpz_powm_ui(result.get_mpz_t(), 
                    base.get_mpz_t(), 
                    pow, 
                    modulo.get_mpz_t());
        if (result == 1)
        {
            gtor = false;
            break;
        }
    }

    return gtor;
}

} // namespace math