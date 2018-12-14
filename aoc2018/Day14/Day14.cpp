#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

std::vector<unsigned> recipes;
unsigned e1 = 0, e2 = 1;

std::string do_part1(unsigned n)
{
    recipes.clear();
    e1 = 0, e2 = 1;

    recipes.push_back(3);
    recipes.push_back(7);

    while (recipes.size() < n + 10)
    {
        unsigned long long total = recipes[e1] + recipes[e2];

        auto d1 = total / 10;
        auto d2 = total % 10;

        if (d1)
            recipes.push_back(d1);
        recipes.push_back(d2);

        e1 = (e1 + recipes[e1] + 1) % recipes.size();
        e2 = (e2 + recipes[e2] + 1) % recipes.size();
    }

    std::ostringstream o;
    for (auto i = n; i < n + 10; i++)
        o << recipes[i];
    return o.str();
}

unsigned do_part2(/*const std::vector<unsigned> &n*/)
{
    recipes.clear();
    e1 = 0, e2 = 1;

    recipes.push_back(3);
    recipes.push_back(7);

    unsigned checked_index = 0;

    for (;;)
    {
        unsigned long long total = recipes[e1] + recipes[e2];

        auto d1 = total / 10;
        auto d2 = total % 10;

        if (d1)
            recipes.push_back(d1);
        recipes.push_back(d2);

        e1 = (e1 + recipes[e1] + 1) % recipes.size();
        e2 = (e2 + recipes[e2] + 1) % recipes.size();

        for (int i = checked_index; i + 5 < recipes.size() - 1; i++)
        {
            if (recipes[i] == 2 && recipes[i + 1] == 9 && recipes[i + 2] == 0 &&
                recipes[i + 3] == 4 && recipes[i + 4] == 3 && recipes[i + 5] == 1)
            {
                return i;
            }
            //if (recipes[i] == 5 && recipes[i + 1] == 9 && recipes[i + 2] == 4 &&
            //    recipes[i + 3] == 1 && recipes[i + 4] == 4)
            //{
            //    return i;
            //}

            checked_index = std::max(checked_index, (unsigned)i);
        }
    }
}

int main()
{
    auto part1 = do_part1(290431);
    auto part2 = do_part2();

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == "1776718175");
    assert(part2 == 20220949);

    return 0;
}
