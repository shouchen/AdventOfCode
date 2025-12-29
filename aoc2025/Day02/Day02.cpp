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

auto is_invalid1(const std::string &s)
{
    return s.length() % 2 == 0 && is_repeats(s, s.length() / 2);
}

auto is_invalid2(const std::string &s)
{
    for (auto replen = 1; replen <= s.length() / 2; replen++)
        if (is_repeats(s, replen))
            return true;

    return false;
}

auto solve(const std::string &filename)
{
    auto retval = std::make_pair(0ULL, 0ULL);
    auto first = 0ULL, second = 0ULL;
    auto dash = '-', comma = ',';

    std::ifstream file(filename);

    while (file >> first >> dash >> second)
    {
        for (auto curr = first; curr <= second; curr++)
        {
            auto s = std::to_string(curr);

            if (is_invalid1(s))
                retval.first += curr;
            if (is_invalid2(s))
                retval.second += curr;
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
