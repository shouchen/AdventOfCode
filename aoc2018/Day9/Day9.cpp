#include "stdafx.h"
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cassert>

auto do_part(unsigned num_players, unsigned last_marble)
{
    std::list<unsigned> circle;
    std::vector<unsigned> score(num_players + 1);

    circle.push_back(0);
    auto current = circle.begin();
    auto elf = 1U, highest_score = 0U;

    for (auto marble = 1U; marble <= last_marble; marble++)
    {
        if (marble % 23)
        {
            if (++current == circle.end())
                current = circle.begin();

            current = circle.insert(++current, marble);
        }
        else
        {
            for (auto i = 0; i < 7; i++)
            {
                if (current == circle.begin())
                    current = circle.end();
                current--;
            }

            score[elf] += marble + *current;
            highest_score = std::max(highest_score, score[elf]);

            current = circle.erase(current);
            if (current == circle.end())
                current = circle.begin();
        }

        if (++elf > num_players)
            elf = 1U;
    }

    return highest_score;
}

int main()
{
    const auto num_players = 459, last_marble_value = 71320;

    auto part1 = do_part(num_players, last_marble_value);
    auto part2 = do_part(num_players, last_marble_value * 100);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 375414);
    assert(part2 == 3168033673); 
    return 0;
}
