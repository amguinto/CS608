#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../utils/Message_utils.hpp"


TEST(test_Message_utils, naive_plaintext_numeric)
{
#ifndef using_ASCII
    // Old version.
    auto naive_old = message::naive_plaintext_numeric("njit");
    EXPECT_EQ(naive_old[0], 14);
    EXPECT_EQ(naive_old[1], 10);
    EXPECT_EQ(naive_old[2], 9);
    EXPECT_EQ(naive_old[3], 20);
    
    auto naive = message::naive_plaintext_numeric("njit", 4);

    // Block size of 4.
    EXPECT_EQ(naive.size(), 1);    // Number of blocks
    EXPECT_EQ(naive[0].size(), 4); // Number of characters per block

    EXPECT_EQ(naive[0][0], 14); // n
    EXPECT_EQ(naive[0][1], 10); // j
    EXPECT_EQ(naive[0][2], 9);  // i
    EXPECT_EQ(naive[0][3], 20); // t

    // for (const auto& block : naive)
    // {
    //     std::cout << "block = ";
    //     for (const auto& c : block)
    //     {
    //         std::cout << c << "\t";
    //     }
    //     std::cout << std::endl;
    // }

    // Block size of 1.
    auto naive1 = message::naive_plaintext_numeric("njit", 1);
    EXPECT_EQ(naive1.size(), 4);    // Number of blocks
    EXPECT_EQ(naive1[0].size(), 1); // Number of characters per block

    EXPECT_EQ(naive1[0][0], 14); // n
    EXPECT_EQ(naive1[1][0], 10); // j
    EXPECT_EQ(naive1[2][0], 9);  // i
    EXPECT_EQ(naive1[3][0], 20); // t


    // Block size of 10, we would have 0 padding.
    auto naive2 = message::naive_plaintext_numeric("njit", 10);
    EXPECT_EQ(naive2.size(), 1);    // Number of blocks
    EXPECT_EQ(naive2[0].size(), 10); // Number of characters per block
    EXPECT_EQ(naive2[0][0], 14); // n
    EXPECT_EQ(naive2[0][1], 10); // j
    EXPECT_EQ(naive2[0][2], 9);  // i
    EXPECT_EQ(naive2[0][3], 20); // t
    EXPECT_EQ(naive2[0][4], 0);  // 0
    EXPECT_EQ(naive2[0][5], 0);  // 0
    EXPECT_EQ(naive2[0][6], 0);  // 0
    EXPECT_EQ(naive2[0][7], 0);  // 0
    EXPECT_EQ(naive2[0][8], 0);  // 0
    EXPECT_EQ(naive2[0][9], 0);  // 0


    // Block size of 3.
    auto naive3 = message::naive_plaintext_numeric("njit", 3);
    EXPECT_EQ(naive3.size(), 2);    // Number of blocks
    EXPECT_EQ(naive3[0].size(), 3); // Number of characters per block

    EXPECT_EQ(naive3[0][0], 14); // n
    EXPECT_EQ(naive3[0][1], 10); // j
    EXPECT_EQ(naive3[0][2], 9);  // i
    EXPECT_EQ(naive3[1][0], 20); // t
    EXPECT_EQ(naive3[1][1], 0);  // 0
    EXPECT_EQ(naive3[1][2], 0);  // 0


    // Block size of 2.
    auto naive4 = message::naive_plaintext_numeric("njit", 2);
    EXPECT_EQ(naive4.size(), 2);    // Number of blocks
    EXPECT_EQ(naive4[0].size(), 2); // Number of characters per block

    EXPECT_EQ(naive4[0][0], 14); // n
    EXPECT_EQ(naive4[0][1], 10); // j
    EXPECT_EQ(naive4[1][0], 9);  // i
    EXPECT_EQ(naive4[1][1], 20); // t
#endif
}

TEST(test_Message_utils, encode_naive_representation)
{
#ifndef using_ASCII
    auto naive = message::naive_plaintext_numeric("njit", 4);
    auto compressed = message::encode_naive_representation(naive);
    EXPECT_EQ(compressed.size(), 1);
    EXPECT_EQ(compressed[0], 315440);


    auto naive1 = message::naive_plaintext_numeric("njit", 1);
    auto compressed1 = message::encode_naive_representation(naive1);
    // Can't compress if only 1 character, so it's itself.
    EXPECT_EQ(compressed1.size(), 4);
    EXPECT_EQ(compressed1[0], 14);
    EXPECT_EQ(compressed1[1], 10);
    EXPECT_EQ(compressed1[2], 9);
    EXPECT_EQ(compressed1[3], 20);

    // Zero padding example.
    auto naive2 = message::naive_plaintext_numeric("njit", 10);
    auto compressed2 = message::encode_naive_representation(naive2);
    EXPECT_EQ(compressed2.size(), 1);
    EXPECT_EQ(compressed2[0], 315440);
#endif
}

TEST(test_Message_utils, decode_naive_representation)
{
#ifndef using_ASCII
    // Deprecated version
    std::string decoded = message::decode_naive_representation(315440);
    EXPECT_EQ(decoded, "njit");


    std::vector<mpz_class> compressed;
    mpz_class block(315440);
    compressed.emplace_back(block);
    std::string m = message::decode_naive_representation(compressed);
    EXPECT_EQ(m, "njit");


    // 1 character per block.
    auto numeric_blocks = message::naive_plaintext_numeric("njit", 1);
    auto compressed_blocks = message::encode_naive_representation(numeric_blocks);
    std::string message = message::decode_naive_representation(compressed_blocks);
    EXPECT_EQ(message, "njit");


    // 2 characters per block.
    auto numeric_blocks2 = message::naive_plaintext_numeric("njit", 2);
    auto compressed_blocks2 = message::encode_naive_representation(numeric_blocks);
    std::string message2 = message::decode_naive_representation(compressed_blocks);
    EXPECT_EQ(message2, "njit");
#endif
}

TEST(test_Message_utils, long_message)
{
    std::string str = "do not send files as attachments to emails";
    auto naive_old = message::naive_plaintext_numeric(str);
    auto compressed_old = message::encode_naive_representation(naive_old);
    std::string decoded_old = message::decode_naive_representation(compressed_old);
    EXPECT_EQ(str, decoded_old);

    auto naive = message::naive_plaintext_numeric(str, 2);
    auto compressed = message::encode_naive_representation(naive);
    std::string decoded = message::decode_naive_representation(compressed);
    EXPECT_EQ(str, decoded);
}

TEST(test_Message_utils, ASCII)
{
#ifdef using_ASCII
    std::string str = "Do not send files";
    auto naive = message::naive_plaintext_numeric(str, 4);
    auto compressed = message::encode_naive_representation(naive);
    std::string decoded = message::decode_naive_representation(compressed);
    EXPECT_EQ(str, decoded);

    std::string str2 = "Do not send files as attachments to emails";
    auto naive2 = message::naive_plaintext_numeric(str2, 1);
    auto compressed2 = message::encode_naive_representation(naive2);
    std::string decoded2 = message::decode_naive_representation(compressed2);
    EXPECT_EQ(str2, decoded2);
    // for (const auto& block : naive)
    // {
    //     std::cout << "block = ";
    //     for (const auto& c : block)
    //     {
    //         std::cout << c << "\t";
    //     }
    //     std::cout << std::endl;
    // }
#endif
}

// TEST(test_Message_utils, Exam)
// {
//     // IMPORTANT: Switch to ASCII or add capital letters to handle capitalization or add capitals to map.
//     // Enter string to convert
//     std::string str = "do not send files";

//     // Create a vector of blocks based on the block size.
//     //! @params:
//     //!          @string: string to create numeric form.
//     //!          @block_size: block size (optional)
//     auto naive = message::naive_plaintext_numeric(str, 4);

//     // Alternatively, if no block size is specified, use the old function.
//     // auto naive_old = message::naive_plaintext_numeric(str);

//     // Compute the compressed version of the numeric form for each block.
//     // This function is overloaded to handle in case no block size is specified.
//     auto compressed = message::encode_naive_representation(naive);

//     // Decode using the map we specified via macros.
//     std::string decoded = message::decode_naive_representation(compressed);
//     EXPECT_EQ(str, decoded);


//     // Use the loop below to view each block in the vector of blocks.
//     // for (const auto& block : naive)
//     // {
//     //     std::cout << "block = ";
//     //     for (const auto& c : block)
//     //     {
//     //         std::cout << c << "\t";
//     //     }
//     //     std::cout << std::endl;
//     // }
// }