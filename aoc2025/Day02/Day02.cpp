#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

auto is_repeats(const std::string &s, int replen)
{
    if (s.length() % replen)
        return false;

    for (auto i = replen; i < s.length(); i++)
        if (s[i] != s[i % replen])
            return false;

    return true;
}

auto is_invalid(unsigned long long n)
{
    auto s = std::to_string(n);

    for (auto replen = s.length() / 2; replen; --replen)
        if (is_repeats(s, replen))
            return std::make_pair(replen + replen == s.length(), true);

    return std::make_pair(false, false);
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    auto retval = std::make_pair(0ULL, 0ULL);
    auto first = 0ULL, second = 0ULL;
    auto dash = '-', comma = ',';

    while (file >> first >> dash >> second)
    {
        for (auto curr = first; curr <= second; curr++)
        {
            auto inv = is_invalid(curr);
            if (inv.first) retval.first += curr;
            if (inv.second) retval.second += curr;
        }

        file >> comma;
    }

    return retval;
}

int main()
{
    auto solution = solve("input.txt");
    std::cout << "Part One: " << solution.first << std::endl;
    std::cout << "Part Two: " << solution.second << std::endl;

    assert(solution.first == 38310256125);
    assert(solution.second == 58961152806);
    return 0;
}
