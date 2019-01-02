#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

using namespace std::string_literals;

auto do_part1(const std::string &input, char exclude = ' ')
{
    auto s = ""s;
    
    for (auto c : input)
    {
        if (tolower(c) == exclude)
            continue;

        if (!s.empty())
        {
            auto prev = s.back();
            if (c != prev && tolower(c) == tolower(prev))
            {
                s.pop_back();
                continue;
            }
        }

        s.push_back(c);
    }

    return static_cast<unsigned>(s.size());
}

auto do_part2(const std::string &input)
{
    auto lowest_size = std::numeric_limits<unsigned>::max();

    for (auto exclude = 'a'; exclude <= 'z'; exclude++)
    {
        auto size = do_part1(input, exclude);
        if (size < lowest_size)
            lowest_size = size;
    }

    return lowest_size;
}

int main()
{
    std::ifstream file("input.txt");
    auto input = ""s;
    file >> input;

    auto part1 = do_part1(input);
    auto part2 = do_part2(input);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 11476);
    assert(part2 == 5446);
    return 0;
}
