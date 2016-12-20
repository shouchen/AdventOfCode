#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cassert>

std::vector<std::pair<unsigned, unsigned>> ranges;

void ReadInput(const std::string &filename)
{
    std::ifstream f(filename);
    unsigned start, end;
    char dash;

    while (f >> start >> dash >> end)
        ranges.push_back(std::pair<unsigned, unsigned>(start, end));
}

bool WayToSort(std::pair<unsigned, unsigned> &p1, std::pair<unsigned, unsigned> &p2)
{
    return p1.first < p2.first;
}

// Returns true if something was coalesced, false if nothing more can be done.
bool CoalesceOneRange()
{
    for (auto a = ranges.begin(); a != ranges.end(); a++)
    {
        // Try to merge qualifying B's with A.
        auto lowestOverlappingBStart = (a->second == UINT_MAX) ? a->second : (a->second + 1);
        for (auto b = a + 1; b != ranges.end() && b->first <= lowestOverlappingBStart; b++)
        {
            // In here, we know and B overlap, and that B's start is not before A's. We can
            // remove B and merge it into A. This can only change A's end range, so the sort
            // order won't be affected.

            // Case 1: B overlaps A, but extends past it. Extend A to the new end and delete B.
            // Case 2: B is wholly contained within A. Just delete B as redundant.

            a->second = std::max(a->second, b->second);
            ranges.erase(b);
            return true;
        }
    }

    return false;
}

unsigned SolvePart1()
{
    return ranges[0].second + 1;
}

unsigned SolvePart2()
{
    unsigned sizeOfRanges = 0;
    for (auto r : ranges)
    {
        auto thisSize = r.second - r.first + 1;
        sizeOfRanges += thisSize;
    }

    return UINT_MAX - sizeOfRanges + 1;
}

int main()
{
    double startTime = clock();

    ReadInput("input.txt");

    std::sort(ranges.begin(), ranges.end(), WayToSort);
    while (CoalesceOneRange());

    auto part1 = SolvePart1();
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 32259706);

    auto part2 = SolvePart2();
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 113);

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;
    return 0;
}

