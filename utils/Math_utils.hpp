#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

// Standard C/C++
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <math.h>
#include <set>
#include <vector>

// GMP
#include <gmpxx.h>

namespace math
{
// Basic Eucliean Algorithm
template <typename T>
static inline T
gcd(const T a, const T b)
{
    if (a == 0)
    {
        return b;
    }
    return gcd(b % a, a);
}

template <typename T>
static inline bool
is_coprime(const T a, const T b)
{
    return (math::gcd(a, b) == 1);
}

// Get all prime factors
// Implicit conversion takes care of smaller integer types.
// Not efficient, but there's not enough type to add template metaprogramming.
template<typename T>
static inline std::vector<T> 
get_prime_factors(T num)
{
    std::vector<T> prime_factors;
    prime_factors.reserve(5);

    while(num % 2 == 0)
    {
        prime_factors.emplace_back(static_cast<T>(2));
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
template<typename T>
static inline std::set<size_t> get_prime_factors_unq(const T num)
{
    mpz_class n(num);
    auto all_primes = get_prime_factors(n.get_ui());
    std::set<std::uint64_t> prime_factors_unq(all_primes.begin(), all_primes.end()); 

    return prime_factors_unq;
}


// Helper function since std::pow isn't overloaded for integer types.
template<typename T>
static inline mpz_class 
IPow(const T base, const T power)
{
    mpz_class rop{};
    mpz_ui_pow_ui(rop.get_mpz_t(), base, power);
    return rop;
}

template<typename T>
static inline T 
find_smallest_generator(const T num, const T exception = 0)
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
        // Have an option to exclude a generator. Incase the output of a function isn't good.
        // i.e. (ElGamal's Mask = 1 mod p)
        if (i == exception)
        {
            continue;
        }

        if (found)
        {
            break;
        }

        // Initialize new generator.
        mpz_class base(i);

        for (const auto& e : factors)
        {
            mpz_class result{};
            mpz_class pow((num - 1) / e);
            mpz_powm_ui(result.get_mpz_t(), 
                        base.get_mpz_t(), 
                        pow.get_ui(), 
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
static inline bool 
is_generator(const size_t num, const size_t g)
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


// TODO: Unify with non primes function
static inline mpz_class 
Euler_Totient_primes(const mpz_class& p,
                     const mpz_class& q)
{
    return (p - 1) * (q - 1);
}

static inline mpz_class
Multiplicative_Inverse(const mpz_class& x, // What we want the inverse of
                       const mpz_class& modulo)
{
    // First we need to check if the gcd(x, modulo) are coprime.
    assert(is_coprime(x, modulo));

    mpz_class result{};

    // Remainder/Quotient pair.
    std::vector<mpz_class> remainders;
    remainders.reserve(5);

    // Put the first x value.
    remainders.emplace_back(x);

    std::vector<mpz_class> quotients;
    quotients.reserve(5);

    mpz_class q{};
    mpz_class r{};

    mpz_class dividend(modulo);
    mpz_class divisor(x);
    while (true)
    {
        mpz_fdiv_qr(q.get_mpz_t(),
                    r.get_mpz_t(),
                    dividend.get_mpz_t(),
                    divisor.get_mpz_t());

        dividend = divisor;
        divisor = r;
        
        remainders.emplace_back(r);
        quotients.emplace_back(q);

        if (r == 0)
        {
            std::cerr << "The inverse does not exist." << std::endl;
            break;
        }
        else if (r == 1)
        {
            mpz_class prev{};
            mpz_class curr(1);
            for (auto i = quotients.crbegin(); i != quotients.crend(); ++i)
            {
                auto old_curr = curr;
                curr = (curr * (*i)) + prev;
                prev = old_curr;
            }

            if (remainders.size() % 2 == 0)
            {
                result = modulo - curr;
            }
            else
            {
                result = curr;
            }

            break;
        }
    }

    // Confirm the inverse is correct.
    mpz_class test_result{};
    mpz_class test_base(result * x);
    mpz_powm_ui(test_result.get_mpz_t(), 
                test_base.get_mpz_t(), 
                1, 
                modulo.get_mpz_t());
    
    assert(test_result == 1);

    return result;
}


} // namespace math
#endif // MATH_UTILS_HPP
