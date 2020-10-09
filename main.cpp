#include <iostream>
#include <map>
#include <string>

// Basic Eucliean Algorithm
size_t gcd(const size_t a, const size_t b)
{
    if (a == 0)
    {
        return b;
    }
    return gcd(b % a, a);
}

static const std::map<char, std::string> numeric_plaintext_map = {
    { 'a', "00" }, { 'b', "01" }, { 'c', "02" }, { 'd', "03" }, 
    { 'e', "04" }, { 'f', "05" }, { 'g', "06" }, { 'h', "07" }, 
    { 'i', "08" }, { 'j', "09" }, { 'k', "10" }, { 'l', "11" }, 
    { 'm', "12" }, { 'n', "13" }, { 'o', "14" }, { 'p', "15" }, 
    { 'q', "16" }, { 'r', "17" }, { 's', "18" }, { 't', "19" }, 
    { 'u', "20" }, { 'v', "21" }, { 'w', "22" }, { 'x', "23" }, 
    { 'y', "24" }, { 'z', "25" }
};

// Helper function to map Numerical_plaintext.
std::string parse_numeric_plaintext(const char& charac)
{
    std::string numeric = "does not exist";
    const auto it = numeric_plaintext_map.find(charac);
    if (it != numeric_plaintext_map.cend())
    {
        numeric = it->second;
    }

    return numeric;
}

// Plaintext Presentation in Numerical Forms
std::string naive_plaintext_numeric(const std::string& str)
{
    std::string numeric_form{};
    for (const auto& c : str)
    {
        numeric_form += parse_numeric_plaintext(c);
    }
    return numeric_form;
}

int main()
{
    auto a = 200;
    auto b = 100;

    std::string example = "ab";
    std::cout << "GCD = " << gcd(a, b) << std::endl;
    std::cout << "Numerical Representation =  " << naive_plaintext_numeric(example) << std::endl;
}