#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cassert>

auto formula1(int input)
{
    return input / 3 - 2;
}

int formula2(int input)
{
    auto f1 = formula1(input);
    return f1 + ((f1 > 6) ? formula2(f1) : 0);
}

auto process(const std::string &filename, int formula(int))
{
    auto total = 0, n = 0;
    std::ifstream file(filename);

    while (file >> n)
        total += formula(n);

    return total;
}

int main()
{
    auto part1 = process("input.txt", formula1);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = process("input.txt", formula2);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 3232358);
    assert(part2 == 4845669);
    return 0;
}
