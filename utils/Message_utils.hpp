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
static const std::map<char, std::size_t> numeric_plaintext_map = {
    { 'a', 0 },  { 'b', 1  }, { 'c', 2 },  { 'd', 3  }, 
    { 'e', 4 },  { 'f', 5  }, { 'g', 6 },  { 'h', 7  }, 
    { 'i', 8 },  { 'j', 9  }, { 'k', 10 }, { 'l', 11 }, 
    { 'm', 12 }, { 'n', 13 }, { 'o', 14 }, { 'p', 15 }, 
    { 'q', 16 }, { 'r', 17 }, { 's', 18 }, { 't', 19 }, 
    { 'u', 20 }, { 'v', 21 }, { 'w', 22 }, { 'x', 23 },
    { 'y', 24 }, { 'z', 25 }, { ' ', 26}
};

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

    const auto it = numeric_plaintext_map.find(charac);
    if (it != numeric_plaintext_map.cend())
    {
        numeric = it->second;
    }
    else
    {
        std::cout << "Ignoring character: " << charac << std::endl;
    }
    
    return numeric;
}

// TODO: Add block sizes here.
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
    }

    return c;
}

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

static inline mpz_class 
encode_naive_representation(const std::vector<std::size_t>& numeric_form)
{
    mpz_class compressed_num{};
    for (std::size_t i = 0, r = numeric_form.size() - 1;
         i <= numeric_form.size() - 1; 
         ++i, --r)
    {
        mpz_class result = numeric_form[i] * math::IPow(numeric_plaintext_map.size(), r);
        compressed_num += result;
    }

    return compressed_num;
}

static inline std::string 
decode_naive_representation(mpz_class compressed_num)
{
    std::string text = "";
    for (;;)
    {
        if (compressed_num < numeric_plaintext_map.size())
        {
            // Most likely last letter.
            text.insert(0, 1, numeric_to_char(compressed_num));
            break;
        }
        auto num = compressed_num % numeric_plaintext_map.size();
        text.insert(0, 1, numeric_to_char(num));
        compressed_num = (compressed_num - num) / numeric_plaintext_map.size();
    }

    return text;
}

} // namespace message
#endif // MESSAGE_UTILS_HPP
