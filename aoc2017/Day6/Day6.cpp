#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

void process_file(const std::string &filename, unsigned &part1, unsigned &part2)
{
    std::vector<int> banks;

    std::ifstream f(filename);
    unsigned count;

    while (f >> count)
        banks.push_back(count);

    std::map<std::vector<int>, unsigned> seen;
    auto iter = 0U;
    auto diff = 0U;
    for (;;)
    {
        auto found = seen.find(banks);
        if (found != seen.end())
        {
            diff = iter - seen[banks];
            part1 = iter;
            part2 = diff;
            return;
        }

        seen[banks] = iter;

        unsigned max = 0;
        for (auto i = 1; i < banks.size(); i++)
        {
            if (banks[i] > banks[max])
                max = i;
        }

        auto over = banks[max];
        banks[max] = 0;
        auto curr = (max + 1) % banks.size();

        while (over)
        {
            banks[curr]++;
            over--;

            curr = (curr + 1) % banks.size();
        }

        ++iter;
    }
}

int main()
{
    auto part1 = 0U, part2 = 0U;
    process_file("input-test.txt", part1, part2);
    assert(part1 == 5);
    assert(part2 == 4);

    process_file("input.txt", part1, part2);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 6681);
    assert(part2 == 2392);
    return 0;
}
