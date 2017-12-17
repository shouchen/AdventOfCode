#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <list>

unsigned do_part1(unsigned input)
{
    std::list<unsigned> list;
    list.push_front(0);

    auto curr = list.begin();

    for (auto i = 1U; i <= 2017; i++)
    {
        for (auto steps = input % i; steps; --steps)
            if (++curr == list.end())
                curr = list.begin();

        curr = list.insert(++curr, i);
    }

    return *(++curr == list.end() ? list.begin() : curr);
}

unsigned do_part2(unsigned input)
{
    // Only need to store what's in index 1 at any given time.
    auto curr = 0U, size = 1U, value_in_index1 = 0U;

    for (auto i = 1U; i <= 50000000; i++)
    {
        curr = (curr + input) % size++;

        if (++curr == 1)
            value_in_index1 = i;
    }

    return value_in_index1;
}

int main()
{
    assert(do_part1(3) == 638);

    const auto input = 344U;
    auto part1 = do_part1(input);
    auto part2 = do_part2(input);

    std::cout << part1 << std::endl;
    std::cout << part2 << std::endl;

    assert(part1 == 996);
    assert(part2 == 1898341);
    return 0;
}

