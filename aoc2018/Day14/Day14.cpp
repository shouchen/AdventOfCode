#include "stdafx.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cassert>

std::string do_part1(unsigned input)
{
    std::string recipes;
    unsigned e1 = 0, e2 = 1;

    recipes.push_back('3');
    recipes.push_back('7');

    while (recipes.length() < input + 10)
    {
        auto total = (recipes[e1] - '0') + (recipes[e2] - '0');

        if (total >= 10)
        {
            recipes.push_back('1');
            total -= 10;
        }

        recipes.push_back(total + '0');

        e1 = (e1 + (recipes[e1] - '0') + 1) % recipes.length();
        e2 = (e2 + (recipes[e2] - '0') + 1) % recipes.length();
    }

    return recipes.substr(input, 10);
}

unsigned do_part2(const std::string &input)
{
    std::string recipes;
    recipes.reserve(21000000);
    unsigned e1 = 0, e2 = 1;

    recipes.push_back('3');
    recipes.push_back('7');

    unsigned checked_index = 0;

    for (;;)
    {
        auto total = (recipes[e1] - '0') + (recipes[e2] - '0');

        if (total >= 10)
        {
            recipes.push_back('1');
            total -= 10;
        }

        recipes.push_back(total + '0');

        e1 = (e1 + (recipes[e1] - '0') + 1) % recipes.length();
        e2 = (e2 + (recipes[e2] - '0') + 1) % recipes.length();

        for (auto i = checked_index; i + input.length() < recipes.length(); i++)
        {
            auto match = true;
            for (auto j = 0; j < input.length(); j++)
            {
                if (recipes[i + j] != input[j])
                {
                    match = false;
                    break;
                }
            }

            if (match)
                return i;

            checked_index = std::max(checked_index, i);
        }
    }
}

int main()
{
    auto part1 = do_part1(290431);
    auto part2 = do_part2("290431");

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == "1776718175");
    assert(part2 == 20220949);

    return 0;
}
