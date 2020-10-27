#ifndef MESSAGE_UTILS_HPP
#define MESSAGE_UTILS_HPP

#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <math.h>
#include <set>
#include <string>
#include <vector>

#include "Math_utils.hpp"

namespace message
{
static const std::map<char, std::size_t> numeric_plaintext_map_basic = {
    { 'a', 1 },  { 'b', 2  }, { 'c', 3 },  { 'd', 4  }, 
    { 'e', 5 },  { 'f', 6  }, { 'g', 7 },  { 'h', 8  }, 
    { 'i', 9 },  { 'j', 10 }, { 'k', 11 }, { 'l', 12 }, 
    { 'm', 13 }, { 'n', 14 }, { 'o', 15 }, { 'p', 16 }, 
    { 'q', 17 }, { 'r', 18 }, { 's', 19 }, { 't', 20 }, 
    { 'u', 21 }, { 'v', 22 }, { 'w', 23 }, { 'x', 24 },
    { 'y', 25 }, { 'z', 26 }, { ' ', 27 }, { '\0',  0 }
};

static const std::map<char, int64_t> numeric_plaintext_map_ascii = {
    { 'a', int('a') }, { 'b', int('b') }, { 'c', int('c') }, { 'd', int('d') },
    { 'e', int('e') }, { 'f', int('f') }, { 'g', int('g') }, { 'h', int('h') },
    { 'i', int('i') }, { 'j', int('j') }, { 'k', int('k') }, { 'l', int('l') },
    { 'm', int('m') }, { 'n', int('n') }, { 'o', int('o') }, { 'p', int('p') },
    { 'q', int('q') }, { 'r', int('r') }, { 's', int('s') }, { 't', int('t') },
    { 'u', int('u') }, { 'v', int('v') }, { 'w', int('w') }, { 'x', int('x') },
    { 'y', int('y') }, { 'z', int('z') }, { ' ', int(' ')},

    { 'A', int('A') }, { 'B', int('B') }, { 'C', int('C') }, { 'D', int('D') }, 
    { 'E', int('E') }, { 'F', int('F') }, { 'G', int('G') }, { 'H', int('H') }, 
    { 'I', int('I') }, { 'J', int('J') }, { 'K', int('K') }, { 'L', int('L') }, 
    { 'M', int('M') }, { 'N', int('N') }, { 'O', int('O') }, { 'P', int('P') }, 
    { 'Q', int('Q') }, { 'R', int('R') }, { 'S', int('S') }, { 'T', int('T') }, 
    { 'U', int('U') }, { 'V', int('V') }, { 'W', int('W') }, { 'X', int('X') },
    { 'Y', int('Y') }, { 'Z', int('Z') }
};


// Custom map
// #define numeric_plaintext_map numeric_plaintext_map_basic
// #define SIZE_numeric_plaintext numeric_plaintext_map_basic.size()

// ASCII
#define using_ASCII // Helper flag.
#define numeric_plaintext_map numeric_plaintext_map_ascii
#define SIZE_numeric_plaintext std::size_t(126) // ASCII



// Example in textbook.
// static const std::map<char, std::size_t> numeric_plaintext_map = {
//     { 'a', 0 },  { 'b', 1  }, { 'c', 2 },  { 'd', 3  }, 
//     { 'e', 4 },  { 'f', 5  }, { 'g', 6 },  { 'h', 7  }, 
//     { 'i', 8 },  { 'j', 9  }, { 'k', 10 }, { 'l', 11 }, 
//     { 'm', 12 }, { 'n', 13 }, { 'o', 14 }, { 'p', 15 }, 
//     { 'q', 16 }, { 'r', 17 }, { 's', 18 }, { 't', 19 }, 
//     { 'u', 20 }, { 'v', 21 }, { 'w', 22 }, { 'x', 23 },
//     { 'y', 24 }, { 'z', 25 }, { ' ', 26 }
// };

// static const std::map<char, std::size_t> numeric_plaintext_map = {
//     { 'A', 0 },  { 'B', 1  }, { 'C', 2 },  { 'D', 3  },
//     { 'E', 4 },  { 'F', 5  }, { 'G', 6 },  { 'H', 7  },
//     { 'I', 8 },  { 'J', 9  }, { 'K', 10 }, { 'L', 11 },
//     { 'M', 12 }, { 'N', 13 }, { 'O', 14 }, { 'P', 15 },
//     { 'Q', 16 }, { 'R', 17 }, { 'S', 18 }, { 'T', 19 },
//     { 'U', 20 }, { 'V', 21 }, { 'W', 22 }, { 'X', 23 },
//     { 'Y', 24 }, { 'Z', 25 }, { ' ', 26}
// };

// static const std::map<char, std::string> numeric_plaintext_map = {
//     { 'a', "00" }, { 'b', "01" }, { 'c', "02" }, { 'd', "03" },
//     { 'e', "04" }, { 'f', "05" }, { 'g', "06" }, { 'h', "07" },
//     { 'i', "08" }, { 'j', "09" }, { 'k', "10" }, { 'l', "11" },
//     { 'm', "12" }, { 'n', "13" }, { 'o', "14" }, { 'p', "15" },
//     { 'q', "16" }, { 'r', "17" }, { 's', "18" }, { 't', "19" },
//     { 'u', "20" }, { 'v', "21" }, { 'w', "22" }, { 'x', "23" },
//     { 'y', "24" }, { 'z', "25" }
// };

static inline std::size_t 
char_to_numeric(const char& charac)
{
    std::size_t numeric = std::numeric_limits<std::size_t>::max();

#ifndef using_ASCII
    const auto it = numeric_plaintext_map.find(std::tolower(charac));
#else
    const auto it = numeric_plaintext_map.find(charac);
#endif
    if (it != numeric_plaintext_map.cend())
    {
        // std::cout << "Character for " << charac << ", is " << it->second << std::endl;
        numeric = it->second;
    }
    // else
    // {
    //     std::cout << "Ignoring character: " << charac << std::endl;
    // }

    return numeric;
}

template<typename T>
static inline char 
numeric_to_char(const T num)
{
    char c{};
    for (const auto& e : numeric_plaintext_map)
    {
        if (e.second == num)
        {
            // std::cout << "Character for " << num << ", is " << e.first << std::endl;
            c = e.first;
            break;
        }
        // else
        // {
        //     std::cout << "Did not find character for " << num << std::endl;
        // }

    }

    return c;
}

static inline std::vector<std::vector<std::size_t>>
naive_plaintext_numeric(const std::string& plaintext, std::size_t block_size)
{
    assert(block_size > 0);

    std::vector<std::vector<std::size_t>> all_blocks;
    all_blocks.reserve(std::ceil(plaintext.size() / block_size));

    std::vector<std::size_t> block;
    block.reserve(block_size);

    // i = index of string
    // b = keep track of block
    for (std::size_t i = 0, b = 0; i < plaintext.size(); ++i, ++b)
    {
        // Get numeric.
        auto num = char_to_numeric(plaintext[i]);
        block.emplace_back(num);
        // std::cout << "placing into block: " << plaintext[i] << std::endl;
        // Reached our block limit.
        if (b >= block_size - 1)
        {
            // std::cout << "reached block limit" << std::endl;
            // Reset block counter to -1 because we are incrementing b after this.
            b = -1;

            // std::cout << "clearing out block after adding character: " << plaintext[i] << std::endl;
            all_blocks.emplace_back(block);
            block.clear();
        }
    }

    // We didn't clear out our block because it's missing some characters.
    // Make sure our last block is padded. Add 0s.
    if (!block.empty())
    {
        while (block.size() < block_size)
        {
            block.emplace_back(0);
        }
        // Place back into vector.
        all_blocks.emplace_back(block);
    }


    return all_blocks;
}

// Deprecated. no block size.
static inline std::vector<std::size_t> 
naive_plaintext_numeric(const std::string& str)
{
    std::vector<std::size_t> numeric_form;
    numeric_form.reserve(str.size() * 2);

    for (const auto& c : str)
    {
        auto val = char_to_numeric(c);
        numeric_form.emplace_back(val);            
    }

    return numeric_form;
}

static inline std::vector<mpz_class>
encode_naive_representation(const std::vector<std::vector<std::size_t>>& all_blocks)
{
    std::vector<mpz_class> all_blocks_compressed;
    all_blocks_compressed.reserve(all_blocks.size());
    // std::cout << all_blocks.size() << std::endl;
    for (const auto& block : all_blocks)
    {
        mpz_class compressed_block{};
        // std::cout << "current block: ";
        std::size_t actual_block_size = block.size() - 1;

        // Get an r value in case we zero padded, and r would be larger than it actually should be.
        if (block.back() == 0)
        {
            for (std::size_t i = 0; i <= block.size() - 1; ++i)
            {
                if (block[i] == 0)
                {
                    // Need to go down because we don't want to include this character.
                    actual_block_size = i - 1;
                    // std::cout << "actual_block_size = " << actual_block_size << std::endl;
                    break;
                }
            }
        }
        for (std::size_t i = 0, r = actual_block_size;
            i <= actual_block_size; 
            ++i, --r)
        {
            // std::cout << block[i] << "\t";
            // r would be very large if we have a 0, so skip it.

            mpz_class result = block[i] * math::IPow(SIZE_numeric_plaintext, r);
            // std::cout << block[i] << "\t*\t" << SIZE_numeric_plaintext << "^" << r << std::endl;
            // std::cout << "result = " << result << "\t";
            compressed_block += result;
        }
        // std::cout << std::endl;
        // Push compressed version to new vector.
        all_blocks_compressed.emplace_back(compressed_block);
    }

    return all_blocks_compressed;
}

// Deprecated. no block size.
static inline mpz_class
encode_naive_representation(const std::vector<std::size_t>& numeric_form)
{
    mpz_class compressed_num{};
    for (std::size_t i = 0, r = numeric_form.size() - 1;
         i <= numeric_form.size() - 1; 
         ++i, --r)
    {
        mpz_class result = numeric_form[i] * math::IPow(SIZE_numeric_plaintext, r);
        compressed_num += result;
    }

    return compressed_num;
}

static inline std::string 
decode_naive_representation(std::vector<mpz_class>& all_blocks_compressed)
{
    std::string text = "";

    // Starting from bottom up will always get the same order of characters 
    // regardless of block size.
    for (auto rit_block = all_blocks_compressed.crbegin();
         rit_block != all_blocks_compressed.crend();
         ++rit_block)
    {
        auto block_value = *rit_block;
        // When we have more than 1 character per block.
        while (block_value > SIZE_numeric_plaintext)
        {
            mpz_class num{};
            mpz_powm_ui(num.get_mpz_t(),
                        block_value.get_mpz_t(),   // Base
                        1, // Exponent is 1
                        mpz_class(SIZE_numeric_plaintext).get_mpz_t());

            text.insert(0, 1, numeric_to_char(num));
            block_value = (block_value - num) / SIZE_numeric_plaintext;
        }

        // Either we have 1 character per block or we're on the last character.
        if (block_value < SIZE_numeric_plaintext)
        {
            // std::cout << "less, block_compressed = " << block_value << std::endl;
            // Most likely last letter.
            text.insert(0, 1, numeric_to_char(block_value));
            // std::cout << "text = " << text << std::endl;
            // std::cout << std::endl;
        }
    }
    return text;
}

// Deprecated. no block size.
static inline std::string 
decode_naive_representation(mpz_class compressed_num)
{
    std::string text = "";
    for (;;)
    {
        if (compressed_num < SIZE_numeric_plaintext)
        {
            // Most likely last letter.
            text.insert(0, 1, numeric_to_char(compressed_num));
            break;
        }
        auto num = compressed_num % SIZE_numeric_plaintext;
        text.insert(0, 1, numeric_to_char(num));
        compressed_num = (compressed_num - num) / SIZE_numeric_plaintext;
    }

    return text;
}
} // namespace message

#undef numeric_plaintext_map
#endif // MESSAGE_UTILS_HPP
