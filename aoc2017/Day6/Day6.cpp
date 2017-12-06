#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

std::vector<int> banks;
//std::vector<int> banks = { 0, 2, 7, 0 };
//std::vector<int> banks = { 4,	1,	15,	12,	0,	9,	9,	5,	5,	8,	7,	3,	14,	5,	12,	3 };

void read_input(const std::string &filename)
{
    std::ifstream f(filename);
    unsigned count;

    while (f >> count)
        banks.push_back(count);
}

int do_part1()
{
    std::set<std::vector<int>> seen;
    auto iter = 0;
    for (;;)
    {
        if (seen.find(banks) != seen.end())
            return iter;

        seen.insert(banks);

        int max = 0;
        for (int i = 0; i < banks.size(); i++)
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

int do_part2()
{
    std::map<std::vector<int>, unsigned> seen;
    auto iter = 0;
    auto diff = 0;
    for (;;)
    {
        auto found = seen.find(banks);
        if (found != seen.end())
        {
            diff = iter - seen[banks];
            return diff;
        }

        seen[banks] = iter;

        int max = 0;
        for (int i = 0; i < banks.size(); i++)
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
    //assert(process_file("input-test.txt", false) == 5);
    //assert(process_file("input-test.txt", true) == 10);

    read_input("input.txt");
    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 6681);
    assert(part2 == 2392);
    return 0;
}
