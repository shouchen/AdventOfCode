#include "stdafx.h"
#include <iostream>
#include <list>
#include <map>
#include <cassert>

unsigned do_work(unsigned players, unsigned last_marble)
{
    std::list<unsigned> circle;
    std::map<unsigned, unsigned> score;

    circle.push_back(0);
    auto current = circle.begin();
    auto elf = 1U;

    for (unsigned marble = 1; marble <= last_marble; marble++)
    {
        if (marble % 23 == 0)
        {
            for (int i = 0; i < 7; i++)
            {
                if (current == circle.begin())
                    current = circle.end();
                current--;
            }

            score[elf] += marble + *current;

            current = circle.erase(current);
            if (current == circle.end())
                current = circle.begin();
        }
        else
        {
            if (++current == circle.end())
                current = circle.begin();

            current = circle.insert(++current, marble);
        }

        if (++elf > players)
            elf = 1U;
    }

    auto highest_score = 0U;
    for (auto item : score)
        if (item.second > highest_score)
            highest_score = item.second;

    return highest_score;
}

int main()
{
    auto part1 = do_work(459, 71320);
    auto part2 = do_work(459, 71320 * 100);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 375414);
    assert(part2 == 3168033673); 
    return 0;
}
