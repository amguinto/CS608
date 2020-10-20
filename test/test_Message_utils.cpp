#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../utils/Message_utils.hpp"


TEST(test_Message_utils, naive_plaintext_numeric)
{
    auto naive = message::naive_plaintext_numeric("njit");
    EXPECT_EQ(naive[0], 13);
    EXPECT_EQ(naive[1], 9);
    EXPECT_EQ(naive[2], 8);
    EXPECT_EQ(naive[3], 19);
}

TEST(test_Message_utils, encode_naive_representation)
{
    auto naive = message::naive_plaintext_numeric("njit");
    auto compressed = message::encode_naive_representation(naive);
    // NOTE: This changes depending on how large the numeric map is. If 25, = 234799
    EXPECT_EQ(compressed, 262675);
}

TEST(test_Message_utils, decode_naive_representation)
{
    // NOTE: This changes depending on how large the numeric map is. If 25, = 234799
    std::string decoded = message::decode_naive_representation(262675);
    EXPECT_EQ(decoded, "njit");
}

TEST(test_Message_utils, long_message)
{
    std::string str = "do not send files as attachments to emails";
    auto naive = message::naive_plaintext_numeric(str);
    auto compressed = message::encode_naive_representation(naive);
    std::string decoded = message::decode_naive_representation(compressed);
    EXPECT_EQ(str, decoded);
}
