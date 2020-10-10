#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

// Basic Eucliean Algorithm
size_t gcd(const size_t a, const size_t b)
{
    if (a == 0)
    {
        return b;
    }
    return gcd(b % a, a);
}

static const std::map<char, std::size_t> numeric_plaintext_map = {
    { 'a', 0 },  { 'b', 1  }, { 'c', 2 },  { 'd', 3  }, 
    { 'e', 4 },  { 'f', 5  }, { 'g', 6 },  { 'h', 7  }, 
    { 'i', 8 },  { 'j', 9  }, { 'k', 10 }, { 'l', 11 }, 
    { 'm', 12 }, { 'n', 13 }, { 'o', 14 }, { 'p', 15 }, 
    { 'q', 16 }, { 'r', 17 }, { 's', 18 }, { 't', 19 }, 
    { 'u', 20 }, { 'v', 21 }, { 'w', 22 }, { 'x', 23 },
    { 'y', 24 }, { 'z', 25 }
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

// Helper function to map Numerical_plaintext.
// std::string parse_numeric_plaintext(const char& charac)
std::size_t char_to_numeric(const char& charac)
{
    std::size_t numeric = std::numeric_limits<std::size_t>::max();

    const auto it = numeric_plaintext_map.find(charac);
    if (it != numeric_plaintext_map.cend())
    {
        numeric = it->second;
    }
    return numeric;
}

// Plaintext Presentation in Numerical Forms
std::vector<std::size_t> naive_plaintext_numeric(const std::string& str)
{
    std::vector<std::size_t> numeric_form;
    numeric_form.reserve(str.size() * 2);

    for (const auto& c : str)
    {
        numeric_form.emplace_back(char_to_numeric(c));
    }

    return numeric_form;
}

// Compressed representation
std::int64_t encode_naive_representation(const std::vector<std::size_t>& numeric_form)
{
    auto compressed_num = 0;
    for (std::size_t i = 0, r = numeric_form.size() - 1;
         i <= numeric_form.size() - 1; 
         ++i, --r)
    {
        compressed_num += numeric_form[i] * std::pow(numeric_plaintext_map.size(), r);
    }

    return compressed_num;
}

std::vector<std::size_t> decode_naive_representation(std::int64_t compressed_num)
{
    std::vector<std::size_t> numeric_form;
    numeric_form.reserve(4);
    while (compressed_num > numeric_plaintext_map.size())
    {
        auto num = compressed_num % numeric_plaintext_map.size();
        numeric_form.push_back(num);
        compressed_num = (compressed_num - num) / numeric_plaintext_map.size();
    }

    return numeric_form;
}

int main()
{
    auto a = 200;
    auto b = 100;

    std::string example = "njit";
    std::cout << "GCD = " << gcd(a, b) << std::endl;

    auto naive = naive_plaintext_numeric(example);

    std::cout << "Numerical Representation = ";
    for (const auto& e : naive)
    {
        std::cout << e;
    }
    std::cout << std::endl;

    auto compressed = encode_naive_representation(naive);
    std::cout << "Encoded compressed Representation = " << compressed << std::endl;

    auto decoded = decode_naive_representation(compressed);
    std::cout << "Numerical Representation after decoding = ";
    for (const auto& e : decoded)
    {
        std::cout << e;
    }
    std::cout << std::endl;
}