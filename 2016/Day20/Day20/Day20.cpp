#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cassert>

std::vector<std::pair<unsigned, unsigned>> ranges;

unsigned Solve1()
{
    unsigned curr = 0;

    bool done = true;
    do
    {
        done = true;
        for (auto r : ranges)
        {
            // If inside a range, bump candidate to one past end of range.
            if (curr >= r.first && curr <= r.second)
            {
                curr = r.second + 1;
                done = false;
            }
        }
    } while (!done);

    return curr;
}

bool WayToSort(std::pair<unsigned, unsigned> &p1, std::pair<unsigned, unsigned> &p2)
{
    return p1.first < p2.first;
}

bool CoalesceRanges()
{
    std::sort(ranges.begin(), ranges.end(), WayToSort);

    for (auto a = ranges.begin(); a != ranges.end(); a++)
    {
        for (auto b = a + 1; b != ranges.end(); b++)
        {
            // Case 1
            if (a->first == b->first && b->second <= a->second)
            {
                ranges.erase(b);
                return true;
            }

            // Case 2
            if (a->first == b->first && b->second > a->second)
            {
                ranges.erase(a);
                return true;
            }

            // Case 3
            if (a->first <= b->first && a->second >= b->second)
            {
                ranges.erase(b);
                return true;
            }

            // Case 4
            if (a->first <= b->first && a->second >= b->first && a->second <= b->second)
            {
                a->second = b->second;
                ranges.erase(b);
                return true;
            }

            // Case 5
            if (b->first == a->second + 1)
            {
                a->second = b->second;
                ranges.erase(b);
                return true;
            }

            // Case 6
            if (b->first > a->second)
                break;
        }
    }

    return false;
}

void Part1Read()
{
    std::ifstream f("input.txt");
    unsigned start, end;
    char dash;

    while (f >> start >> dash >> end)
        ranges.push_back(std::pair<unsigned, unsigned>(start, end));
}

unsigned Solve2()
{
    unsigned sizeOfRanges = 0;
    for (auto r : ranges)
    {
        auto thisSize = r.second - r.first + 1;
        std::cout << r.first << "-" << r.second << " (" << thisSize << ")" << std::endl;
        sizeOfRanges += thisSize;
    }

    return UINT_MAX - sizeOfRanges + 1;
}

int main()
{
    Part1Read();

    while (CoalesceRanges());

    auto part1 = Solve1();
    auto part2 = Solve2();

    assert(part1 == 32259706);
    assert(part2 == 113);

    return 0;
}

