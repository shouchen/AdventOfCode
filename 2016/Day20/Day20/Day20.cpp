#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cassert>

// Summary of Solution:
// Read the input then coalesce blacklist ranges into a simpler representation.
// Part1 will be the first value after the lowest range ends. For part2, add up
// the size of all the ranges and subtract this from the total address space.

std::vector<std::pair<unsigned, unsigned>> ranges;

void ReadInput(const std::string &filename)
{
    std::ifstream f(filename);
    unsigned start, end;
    char dash;

    while (f >> start >> dash >> end)
        ranges.push_back(std::pair<unsigned, unsigned>(start, end));
}

void CoalesceRanges()
{
    // The logic in this function needs the input to be sorted by start value.
    std::sort(ranges.begin(), ranges.end(),
              [](const auto &p1, const auto &p2) -> bool
    {
        return p1.first < p2.first;
    });

    for (auto a = ranges.begin(); a != ranges.end(); a++)
    {
        // See if next range needs to merge with A (loops because of the erase).
        for (auto b = a + 1; b != ranges.end(); b = a + 1)
        {
            // If this b doesn't qualify for merging, then no other B will.
            if (a->second < UINT_MAX && b->first > a->second + 1)
                break;

            // Here, we know A and B overlap, and B's start is not before A's.
            // We can just remove B and adjust A. Since this can only change A's
            // end range, the existing sort order won't be affected.
            a->second = std::max(a->second, b->second);
            ranges.erase(b);
        }
    }
}

void Solve(unsigned &part1, unsigned &part2)
{
    CoalesceRanges();

    unsigned numBlacklistedAddresses = 0;
    for (auto r : ranges)
        numBlacklistedAddresses += r.second - r.first + 1;

    part1 = ranges.begin()->second + 1;
    part2 = UINT_MAX - numBlacklistedAddresses + 1;
}

int main()
{
    double startTime = clock();

    ReadInput("input.txt");

    auto part1 = 0U, part2 = 0U;
    Solve(part1, part2);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;

    assert(part1 == 32259706);
    assert(part2 == 113);
    return 0;
}
