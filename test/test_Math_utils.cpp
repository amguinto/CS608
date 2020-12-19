#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../utils/Math_utils.hpp"


TEST(test_Math_utils, find_smallest_generator)
{
    EXPECT_EQ(math::find_smallest_generator(307), 5);
    EXPECT_EQ(math::find_smallest_generator(747073), 5);

}

TEST(test_Math_utils, is_generator)
{
    EXPECT_FALSE(math::is_generator(307, 2));
    EXPECT_FALSE(math::is_generator(307, 3));
    EXPECT_FALSE(math::is_generator(307, 4));
    EXPECT_TRUE(math::is_generator(307, 5));
    EXPECT_FALSE(math::is_generator(747073, 2));
    EXPECT_TRUE(math::is_generator(747073, 5));
}

TEST(test_Math_utils, Multiplicative_Inverse)
{
    auto inverse = math::Multiplicative_Inverse(1001, 1024);
    EXPECT_EQ(inverse, 89);

    auto inverse2 = math::Multiplicative_Inverse(117, 125);
    EXPECT_EQ(inverse2, 78);

    auto inverse3 = math::Multiplicative_Inverse(16, 21);
    EXPECT_EQ(inverse3, 4);

    auto inverse4 = math::Multiplicative_Inverse(1973, 2004);
    EXPECT_EQ(inverse4, 905);
}

TEST(test_Math_utils, Square_Roots_Modulo)
{
    // std::vector<mpz_class> sq_roots = math::Square_Roots_Modulo(7,  // p (modulo)
    //                                                             2); // a

    mpz_class modulo = 3571;
    mpz_class a = 2;
    std::vector<mpz_class> sq_roots = math::Square_Roots_Modulo(modulo,  // p (modulo)
                                                                a); // a

    // This returns 0 if no roots.
    for (const auto& root : sq_roots)
    {
        std::cout << "(a = " << a << ") Root of " << modulo << ": " << root << std::endl;
    }
}

TEST(test_Math_utils, Find_All_Points_ECC)
{
    mpz_class modulo = 3571;
    mpz_class a = 7;
    mpz_class b = 15;

    std::vector<std::pair<mpz_class, mpz_class>> 
    coordinates = math::Find_All_Points_ECC(modulo, a, b);


    // This returns 0 if no roots.
    // for (const auto& coords : coordinates)
    // {
    //     std::cout << "modulo = " << modulo << "\ta = " << a << "\tb = " << b << std::endl;
    //     std::cout << "coordinates: (" << coords.first << ", " << coords.second << ")" << std::endl;
    //     std::cout << std::endl;
    // }
}

TEST(test_Math_utils, Add_points_to_ECC)
{
    mpz_class x_1(6);
    mpz_class y_1(62);

    mpz_class x_2(9);
    mpz_class y_2(2377);

    std::pair<mpz_class, mpz_class> P = std::make_pair(x_1, y_1);
    std::pair<mpz_class, mpz_class> Q = std::make_pair(x_2, y_2);

    auto new_points = math::Add_points_to_ECC(Q, P, 3571, 7);
    std::cout << "x = " << new_points.first << "\t y = " << new_points.second << std::endl;
}

TEST(test_Math_utils, Scalar_mult_points_to_ECC)
{
    mpz_class x_1(4);
    mpz_class y_1(14);

    mpz_class k = 3;
    mpz_class modulo = 23;
    mpz_class a = 2;
    std::pair<mpz_class, mpz_class> P = std::make_pair(x_1, y_1);

    auto new_points = math::Scalar_mult_points_to_ECC(P, k, modulo, a);
    std::cout << "x = " << new_points.first << "\t y = " << new_points.second << std::endl;
}
