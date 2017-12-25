#include "stdafx.h"
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

void foo(const std::string &filename, int &part1, int &part2)
{
    part1 = part2 = INT_MIN;

    std::map<std::string, int> registers;

    std::ifstream f(filename);
    std::string line;

    while (std::getline(f, line))
    {
        std::stringstream ss(line);
        std::string reg1, inc_dec, if_, reg2, cond;
        auto amount1 = 0, amount2 = 0;

        ss >> reg1 >> inc_dec >> amount1 >> if_ >> reg2 >> cond >> amount2;

        auto reg2_value = registers[reg2];

        if ((cond == ">") && (reg2_value > amount2) ||
            (cond == "<") && (reg2_value < amount2) ||
            (cond == "==") && (reg2_value == amount2) ||
            (cond == "<=") && (reg2_value <= amount2) ||
            (cond == ">=") && (reg2_value >= amount2) ||
            (cond == "!=") && (reg2_value != amount2))
        {
            registers[reg1] += (inc_dec == "inc") ? amount1 : -amount1;
            part2 = std::max(registers[reg1], part2);
        }
    }

    for (auto item : registers)
        part1 = std::max(part1, item.second);
}

int main()
{
    auto part1 = 0, part2 = 0;
    foo("input-test.txt", part1, part2);

    assert(part1 == 1);
    assert(part2 == 10);

    foo("input.txt", part1, part2);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 7296);
    assert(part2 == 8186);
    return 0;
}
