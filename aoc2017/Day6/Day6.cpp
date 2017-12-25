#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

void process_file(const std::string &filename, unsigned &part1, unsigned &part2)
{
    // Read the input.
    std::vector<unsigned> banks;
    std::ifstream f(filename);
    auto blocks = 0U;

    while (f >> blocks)
        banks.push_back(blocks);

    // Create a place to save the configuration history.
    std::map<std::vector<unsigned>, unsigned> seen;

    // Do process repeatedly until we see a repeated configuration.
    for (part1 = 0U; seen.find(banks) == seen.end(); part1++)
    {
        // Record what we've seen and when.
        seen[banks] = part1;

        // Redistribute the max bank's blocks.
        auto max_bank = std::max_element(banks.begin(), banks.end());
        auto dividend = unsigned(*max_bank / banks.size());
        auto remainder = unsigned(*max_bank % banks.size());
        *max_bank = 0;

        auto i = max_bank;
        do
        {
            // Wrap around circularly.
            if (++i == banks.end())
                i = banks.begin();

            // Dividend blocks go to every bank.
            (*i) += dividend;

            // Remainder blocks go to banks immediately after max (until exhausted).
            if (remainder)
            {
                (*i)++;
                remainder--;
            }
        } while (i != max_bank);
    }

    part2 = part1 - seen[banks];
}

int main()
{
    auto part1 = 0U, part2 = 0U;
    process_file("input-test.txt", part1, part2);
    assert(part1 == 5);
    assert(part2 == 4);

    process_file("input.txt", part1, part2);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 6681);
    assert(part2 == 2392);
    return 0;
}
