#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cassert>
#include <climits>

struct Disc
{
    Disc(unsigned long long numPositions, unsigned long long currPosition) : numPositions(numPositions), currPosition(currPosition) {}
    unsigned long long numPositions;
    unsigned long long currPosition;
};

std::map<int, Disc *> discs;

void ReadFile(const std::string &filename)
{
    std::ifstream f(filename);
    std::string disc, has, positions, at, time, it, is, position;
    char pound, period;
    int d, p, startPos = 0;

    while (f >> disc >> pound >> d >> has >> p >> positions >> at >> time >> it >> is >> at >> position >> startPos >> period)
    {
        Disc *tempDisc = new Disc(p, startPos);
        discs[d] = tempDisc;
    }
}

inline bool IsSolution(unsigned long long time)
{
    for (auto p : discs)
        if ((time + p.first + p.second->currPosition) % p.second->numPositions != 0)
            return false;
    return true;
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

    Disc *tempDisc = new Disc(11, 0);
    discs[7] = tempDisc;

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
