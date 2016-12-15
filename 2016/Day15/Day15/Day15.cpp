#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

class Disc
{
public:
    Disc(unsigned number, unsigned positions, unsigned startPosition)
        : number(number), positions(positions), startPosition(startPosition) {}

    inline bool IsAlignedAtTime(unsigned long long t)
    {
        return ((startPosition + number + t) % positions) == 0;
    }

private:
    unsigned number, positions, startPosition;
};

class Machine
{
public:
    Machine(const std::string &filename)
    {
        std::ifstream f(filename);
        std::string disc, has, positions, at, timeEqualsZero, it, is, position;
        char pound, period;
        unsigned num, numPositions, startPosition;

        while (f >> disc >> pound >> num >> has >> numPositions >> positions >> at
                 >> timeEqualsZero >> it >> is >> at >> position >> startPosition >> period)
        {
            AddDisc(numPositions, startPosition);
        }
    }

    unsigned long long FindSolution()
    {
        auto curr = 0ULL;
        while (!IsSolution(curr))
            curr++;

        return curr;
    }

    void AddDisc(unsigned numPositions, unsigned startPosition)
    {
        discs.push_back(Disc(discs.size() + 1, numPositions, startPosition));
    }

private:
    inline bool IsSolution(unsigned long long time)
    {
        for (auto disc : discs)
            if (!disc.IsAlignedAtTime(time))
                return false;

        return true;
    }

    std::vector<Disc> discs;
};


int main()
{
    Machine machine("input.txt");

    auto part1 = machine.FindSolution();

    machine.AddDisc(11, 0);

    auto part2 = machine.FindSolution();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 122318);
    assert(part2 == 3208583);
    return 0;
}
