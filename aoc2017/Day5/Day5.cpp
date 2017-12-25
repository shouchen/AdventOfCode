#include "stdafx.h"
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

auto process_file(const std::string &filename, bool for_part2)
{
    std::ifstream f(filename);
    std::vector<int> data;
    auto offset = 0;

    while (f >> offset)
        data.push_back(offset);

    auto count = 0U;

    for (auto curr = 0; curr >= 0 && curr < (int)data.size(); curr += offset, count++)
    {
        offset = data[curr];
        data[curr] += (offset >= 3 && for_part2) ? -1 : 1;
    }

    return count;
}

int main()
{
    assert(process_file("input-test.txt", false) == 5);
    assert(process_file("input-test.txt", true) == 10);

    auto part1 = process_file("input.txt", false);
    auto part2 = process_file("input.txt", true);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 381680);
    assert(part2 == 29717847);
    return 0;
}
