#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

auto is_invalid1(const unsigned long long n)
{
    auto s = std::to_string(n);
    auto len = s.length();
    if (len % 2 != 0)
        return false;

    for (auto i = 0; i < len / 2; i ++)
    {
        if (s[i] != s[i + len / 2])
            return false;
    }

    return true;
}

auto is_invalid2(const unsigned long long n)
{
    auto s = std::to_string(n);

    for (auto replen = 1; replen <= s.length() / 2; replen++)
    {
        if (s.length() % replen != 0)
            continue;

        bool repeats = true;
        for (auto i = 0; i < s.length(); i++)
        {
            if (s[i] != s[i % replen])
            {
                repeats = false;
                break;
            }
        }

        if (repeats)
            return true;
    }

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
            if (is_invalid1(curr))
                retval.first += curr;
            if (is_invalid2(curr))
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
