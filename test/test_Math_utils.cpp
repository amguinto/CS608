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
    std::vector<mpz_class> sq_roots = math::Square_Roots_Modulo(7,  // p (modulo)
                                                                3); // a

    // This returns 0 if no roots.
    for (const auto& root : sq_roots)
    {
        std::cout << root << std::endl;
    }
}