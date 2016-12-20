#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cassert>

// Summary:
// Read the input then coalesce the blacklist ranges into a simpler representation. Part1 will
// be the value after the lowest range ends. For part2, add up the size of all the ranges and
// subtract this from the total address space.

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
    std::sort(
        ranges.begin(), ranges.end(),
        [](const std::pair<unsigned, unsigned> &p1, const std::pair<unsigned, unsigned> &p2) -> bool
    {
        return p1.first < p2.first;
    });

    for (auto a = ranges.begin(); a != ranges.end(); a++)
    {
        // Look at next range to see if it needs to be merged with A. It's a loop because of the erase().
        for (auto b = a + 1; b != ranges.end(); b = a + 1)
        {
            // If this b doesn't qualify for merging, then no other B will.
            auto lowestOverlappingBStart = (a->second == UINT_MAX) ? a->second : (a->second + 1);
            if (b->first > lowestOverlappingBStart)
                break;

            // Here, we know that A and B overlap, and that B's start is not before A's. We can remove B
            // and merge it into A. This can only change A's end range, so the sort order won't be affected.
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

    assert(part1 == 32259706);
    assert(part2 == 113);

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;
    return 0;
}
