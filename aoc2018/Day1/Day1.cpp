#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>
#include <set>
#include <cassert>

std::vector<long> input;

void read_input_into_vector(const std::string &filename)
{
    std::ifstream file(filename);
    auto a = 0L;

    while (file >> a)
    {
        input.push_back(a);
    }
}

long do_part1()
{
    return std::accumulate(input.begin(), input.end(), 0);
}

long do_part2()
{
    std::set<long> seen;
    auto current = 0L;

    while (true)
    {
        for (auto frequency : input)
        {
            if (seen.find(current) != seen.end())
            {
                return current;
            }

            seen.insert(current);

            current += frequency;
        }
    }
}

int main()
{
    read_input_into_vector("input.txt");

    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 561);
    assert(part2 == 563);
    return 0;
}
