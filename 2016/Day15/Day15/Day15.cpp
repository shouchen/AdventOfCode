#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

struct Disc
{
    unsigned long long numPositions, currPosition;
};

std::vector<Disc> discs;

inline bool IsSolution(unsigned long long time)
{
    for (auto i = 0U; i < discs.size(); i++)
        if ((time + (i + 1) + discs[i].currPosition) % discs[i].numPositions != 0)
            return false;
    return true;
}

void ReadFile(const std::string &filename)
{
    std::ifstream f(filename);
    std::string disc, has, positions, at, time, it, is, position;
    char pound, period;
    unsigned d, p, startPos = 0;

    while (f >> disc >> pound >> d >> has >> p >> positions >> at >> time >> it
             >> is >> at >> position >> startPos >> period)
    {
        discs.push_back(Disc { p, startPos });
    }
}

int main()
{
    ReadFile("input.txt");

    auto part1 = 0ULL;
    for (;;)
    {
        if (IsSolution(part1))
            break;
        part1++;
    }

    discs.push_back(Disc { 11, 0 });

    auto part2 = 0ULL;
    for (;;)
    {
        if (IsSolution(part2))
            break;
        part2++;
    }

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 122318);
    assert(part2 == 3208583);
    return 0;
}
