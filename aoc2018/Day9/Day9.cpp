#include "stdafx.h"
#include <iostream>
#include <list>
#include <map>
#include <cassert>

unsigned do_work(unsigned players, unsigned last_marble)
{
    std::list<unsigned> circle;
    unsigned marble = 0;
    std::map<unsigned, unsigned> score;

    circle.push_back(marble++);
    auto current = circle.begin();

    for (;;)
    {
        for (auto elf = 1U; elf <= players; elf++)
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
                auto temp = current;
                if (++current == circle.end())
                    current = circle.begin();

                circle.erase(temp);
                marble++;
            }
            else
            {
                if (++current == circle.end())
                    current = circle.begin();

                current = circle.insert(++current, marble++);
            }

            if (marble > last_marble)
                break;
        }

        if (marble > last_marble)
            break;
    }

    unsigned highest_score = 0;
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
