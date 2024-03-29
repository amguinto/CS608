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

static inline mpz_class
Square_and_Multiply_Exponentiation(const mpz_class& modulo,
                                         mpz_class& base,
                                         mpz_class& exponent)
{
    mpz_class A(1);
    while (exponent > 1)
    {
        auto b = exponent % 2;
        exponent = (exponent - b) / 2;
        mpz_powm_ui(base.get_mpz_t(),
                    base.get_mpz_t(),
                    2, // exponent
                    modulo.get_mpz_t());

        if (b == 1)
        {
            mpz_class temp_base(A * base);
            mpz_powm_ui(A.get_mpz_t(),
                        temp_base.get_mpz_t(),
                        1, // exponent
                        modulo.get_mpz_t());
        }
    }

    return A;
}

//! @description: Goal: Solve x^2 = a mod p or r = sqrt(a) mod p
//!                 or how to find r = sqrt(a) mod p
//!               ---------------------------------------
//!               if p mod 4 = 3 and a has a square root,
//!               then r = a^( (p + 1) / 4 ) mod p

// NOTE: p = modulo
//       a = the number we're trying to find the sqrt of
static inline std::vector<mpz_class>
Square_Roots_Modulo(const mpz_class& modulo,
                    const mpz_class& a)
{
    // Check if p mod 4 = 3
    mpz_class check_case{};
    mpz_powm_ui(check_case.get_mpz_t(),  // buffer
                modulo.get_mpz_t(),    // base
                1,                   // exponent
                mpz_class(4).get_mpz_t()); // modulo
    
    if (check_case != 3)
    {
        std::cout << "we DID NOT pass the case where p mod 4 = 3." << std::endl;
    }

    mpz_class a_mod{};
    mpz_powm_ui(a_mod.get_mpz_t(),   // buffer
                a.get_mpz_t(),       // base
                1,                   // exponent
                modulo.get_mpz_t()); // modulo
    
    // x^2 = base % modulo
    // find r = a^( (p + 1) / 4 ) mod p
    mpz_class r{};

    mpz_class base{};
    mpz_ui_pow_ui(base.get_mpz_t(), a.get_ui(), (modulo.get_ui() + 1 ) / 4);
    mpz_powm_ui(r.get_mpz_t(),  // buffer
                base.get_mpz_t(),    // base
                1,                   // exponent
                modulo.get_mpz_t()); // modulo

    // std::cout << "r = " << r << std::endl;

    // NOTE: -r + modulo = another root
    mpz_class r_2(-r + modulo);

    // Now we need to verify the 2 roots we found.
    // Plug it in for x for: x^2 mod p. If it does not equal to a, then the square root does not exit
    mpz_class r_check{};
    mpz_class base_r(r * r); // r^2
    mpz_powm_ui(r_check.get_mpz_t(), // buffer
                base_r.get_mpz_t(),  // base
                1,                   // exponent
                modulo.get_mpz_t()); // modulo

    std::vector<mpz_class> roots{};
    if (r_check == a_mod)
    {
        roots.push_back(r);
    }
    // std::cout << "r_check = " << r_check << std::endl; 
    mpz_class r2_check{};
    mpz_class base_r2(r_2 * r_2); // r^2
    mpz_powm_ui(r2_check.get_mpz_t(), // buffer
                base_r2.get_mpz_t(),  // base
                1,                   // exponent
                modulo.get_mpz_t()); // modulo

    // std::cout << "r_2 = " << r_2 << std::endl;

    if (r2_check == a_mod)
    {
        roots.push_back(r_2);
    }

    if (roots.empty())
    {
        roots.push_back(0);
    }

    return roots;
}

static inline std::vector<std::pair<mpz_class, mpz_class>>
Find_All_Points_ECC(const mpz_class& modulo,
                    const mpz_class& a,
                    const mpz_class& b)
{
    // NOTE: x = i in this case since we iterate to modulo-1.
    std::vector<std::pair<mpz_class, mpz_class>> coordinates;
    // coordinates.reserve(100);

    for (size_t i = 0; i < modulo; ++i)
    {
        // std::cout << "i = " << i << std::endl;
        mpz_class base( (i * i * i) + (a * i) + b); // x^3 + ax + b
        // std::cout << "base = " << base << std::endl;
        std::vector<mpz_class> roots = math::Square_Roots_Modulo(modulo,
                                                                 base);
        for (const auto& val : roots)
        {
            // Make sure the root exists.
            if (val != 0)
            {
                coordinates.push_back(std::make_pair(i, val));
            }
        }
    }

    return coordinates;
}

static inline std::pair<mpz_class, mpz_class>
Add_points_to_ECC(const std::pair<mpz_class, mpz_class>& P,
                  const std::pair<mpz_class, mpz_class>& Q,
                  const mpz_class modulo,
                  const mpz_class a)
{
    mpz_class answer;

    mpz_class x_r;
    mpz_class y_r;
    if (P.first == Q.first and P.second == Q.second)
    {
        mpz_class foo;
        mpz_ui_pow_ui(foo.get_mpz_t(), Q.first.get_ui(), 2);

        mpz_class mult_inverse_foo;

        mpz_class temp_Q = Q.second * mpz_class(2);
        mpz_invert(mult_inverse_foo.get_mpz_t(), temp_Q.get_mpz_t(), modulo.get_mpz_t());
        mpz_class beta = ((3 * (foo) + a) * mult_inverse_foo);
        // mpz_class beta = ((3 * (foo) + a) * math::Multiplicative_Inverse(mpz_class(2) * Q.second, modulo));

        mpz_class beta_powered_2;
        mpz_ui_pow_ui(beta_powered_2.get_mpz_t(), beta.get_ui(), 2);
        mpz_class base_x = beta_powered_2 - 2 * P.first;
        // mpz_class base_x = math::IPow(beta, mpz_class(2)) - (2 * P.first);
        mpz_powm_ui(x_r.get_mpz_t(), // buffer
                    base_x.get_mpz_t(),  // base
                    1,                   // exponent
                    modulo.get_mpz_t()); // modulo

        mpz_class base_y = beta * (P.first - x_r) - P.second;
        mpz_powm_ui(y_r.get_mpz_t(), // buffer
                    base_y.get_mpz_t(),  // base
                    1,                   // exponent
                    modulo.get_mpz_t()); // modulo
    }
    else
    {
        mpz_class mult_inverse_foo;
        mpz_class temp_Q = P.first - Q.first;
        mpz_invert(mult_inverse_foo.get_mpz_t(), temp_Q.get_mpz_t(), modulo.get_mpz_t());
        
        // mpz_class alpha = (P.second - Q.second) * math::Multiplicative_Inverse(P.first - Q.first, modulo);
        mpz_class alpha = (P.second - Q.second) * mult_inverse_foo;

        mpz_class foo;
        mpz_ui_pow_ui(foo.get_mpz_t(), alpha.get_ui(), 2);

        // mpz_class base_x = math::IPow(alpha, mpz_class(2)) - Q.second - P.first;
        mpz_class base_x = foo - Q.first - P.first;

        mpz_powm_ui(x_r.get_mpz_t(), // buffer
                    base_x.get_mpz_t(),  // base
                    1,                   // exponent
                    modulo.get_mpz_t()); // modulo

        mpz_class base_y = alpha * (Q.first - x_r) - Q.second;
        mpz_powm_ui(y_r.get_mpz_t(), // buffer
                    base_y.get_mpz_t(),  // base
                    1,                   // exponent
                    modulo.get_mpz_t()); // modulo
    }

    return std::make_pair(x_r, y_r);
}

static inline std::pair<mpz_class, mpz_class>
Scalar_mult_points_to_ECC(const std::pair<mpz_class, mpz_class>& Q,
                          const mpz_class k,
                          const mpz_class modulo,
                          const mpz_class a)
{
    auto new_Q = std::make_pair(Q.first, Q.second);
    auto P = std::make_pair(Q.first, Q.second);
    for (size_t i = 0; i < k-1; ++i)
    {
        new_Q = math::Add_points_to_ECC(P, new_Q, modulo, a);
        // std::cout << "new_Q: " << new_Q.first << "\t" << new_Q.second << std::endl; 
    }

    return new_Q;
}

// static inline mpz_class
// Order_of_points_ECC(const std::pair<mpz_class, mpz_class>& Q)
// {

// }
} // namespace math
#endif // MATH_UTILS_HPP
