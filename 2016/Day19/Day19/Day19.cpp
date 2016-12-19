#include "stdafx.h"
#include <iostream>
#include <cassert>

#define MAX 3001330

int elves[MAX];

unsigned GetNextIndex(unsigned curr)
{
    for (;;)
    {
        if (++curr == MAX)
            curr = 0;

        if (elves[curr] > 0)
            return curr;
    }
}

int main()
{
    for (auto i = 0; i < MAX; i++)
        elves[i] = 1;

    auto curr = 0U;

    for (;;)
    {
        // Do a step
        auto next = GetNextIndex(curr);
        elves[curr] += elves[next];

        if (elves[curr] == MAX)
            break;

        elves[next] = 0;

        // Advance to next curr one
        curr = GetNextIndex(curr);
    }

    auto part1 = curr + 1;
    std::cout << "Part One: " << part1 << std::endl;

    assert(part1 == 1808357);
    return 0;
}
