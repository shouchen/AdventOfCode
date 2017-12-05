#include "stdafx.h"
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

unsigned do_part1(const std::string &filename)
{
    std::ifstream f(filename);
    int offset;
    std::vector<int> data;

    while (f >> offset)
        data.push_back(offset);

    int curr = 0;
    unsigned count = 0;
    for (;;)
    {
        offset = data[curr];
        data[curr]++;

        curr += offset;
        count++;

        if (curr < 0 || curr >= (int)data.size())
        {
            return count;
        }
    }
}

unsigned do_part2(const std::string &filename)
{
    std::ifstream f(filename);
    int offset;
    std::vector<int> data;

    while (f >> offset)
        data.push_back(offset);

    int curr = 0;
    unsigned count = 0;
    for (;;)
    {
        offset = data[curr];

        if (offset >= 3)
            data[curr]--;
        else
            data[curr]++;

        curr += offset;
        count++;

        if (curr < 0 || curr >= (int)data.size())
        {
            return count;
        }
    }
}

int main()
{
    assert(do_part1("input-test.txt") == 5);
    assert(do_part2("input-test.txt") == 10);

    auto part1 = do_part1("input.txt");
    auto part2 = do_part2("input.txt");

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 381680);
    assert(part2 == 29717847);
    return 0;
}
