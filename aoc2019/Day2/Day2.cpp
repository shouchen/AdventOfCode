#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

std::vector<int> data;

void load_program(const std::string &filename)
{
    std::ifstream file(filename);
    auto a = 0;
    auto comma = ',';

    while (file >> a)
    {
        data.push_back(a);
        file >> comma;
    }
}

auto do_part1(int first, int second)
{
    data.clear();
    load_program("input.txt");

    auto curr = 0;
    data[1] = first;
    data[2] = second;

    for (;;)
    {
        switch (data[curr])
        {
        case 1:
            data[data[curr + 3]] = data[data[curr + 1]] + data[data[curr + 2]];
            break;

        case 2:
            data[data[curr + 3]] = data[data[curr + 1]] * data[data[curr + 2]];
            break;

        case 99:
            return data[0];
        }
        curr += 4;
    }
}

auto do_part2(int target)
{
    for (auto noun = 0; noun <= 99; noun++)
        for (auto verb = 0; verb <= 99; verb++)
            if (do_part1(noun, verb) == target)
                return 100 * noun + verb;

    return -1;
}

int main()
{
    auto part1 = do_part1(12, 2);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2(19690720);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 12490719);
    assert(part2 == 2003);
    return 0;
}
