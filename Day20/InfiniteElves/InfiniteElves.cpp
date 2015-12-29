// InfiniteElves.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <cassert>

std::vector<unsigned> Factorize(unsigned number)
{
    std::vector<unsigned> retval;
    unsigned mid = static_cast<unsigned>(sqrt(number));

    for (auto i = 1U; i <= mid; i++)
        if ((number % i) == 0)
        {
            retval.push_back(i);
            if (number != 1) retval.push_back(number / i);
        }

    return retval;
}

struct PartTwoRule
{
    PartTwoRule(unsigned house, unsigned elfLimit) : house(house), elfLimit(elfLimit) {}
    int operator()(int l, int r) { return (house <= elfLimit * r) ? (l + r) : l; }
    unsigned house, elfLimit;
};

void Solve(int input, unsigned &part1, unsigned &part2)
{
    part1 = part2 = 0U;

    for (auto house = 0U; house < UINT_MAX; house++)
    {
        auto factors = Factorize(house);

        if (!part1 && std::accumulate(factors.begin(), factors.end(), 0) * 10 >= input)
        {
            part1 = house;
            if (part2) break;
        }

        if (!part2 && std::accumulate(factors.begin(), factors.end(), 0, PartTwoRule(house, 50)) * 11 >= input)
        {
            part2 = house;
            if (part1) break;
        }
    }
}

void _tmain(int argc, _TCHAR *argv[])
{
    auto part1 = 0U, part2 = 0U;
    Solve(33100000, part1, part2);

    std::cout << "part one: " << part1 << std::endl;
    assert(part1 == 776160);

    std::cout << "part two: " << part2 << std::endl;
    assert(part2 == 786240);
}
