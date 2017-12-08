#include "stdafx.h"
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

std::map<std::string, int> registers;

int main()
{
    std::ifstream f("input.txt");
    std::string line;

    int part2 = INT_MIN;

    while (std::getline(f, line))
    {
        std::stringstream ss(line);
        std::string reg1, reg2, inc_dec, if_, cond;
        int amount1, amount2;

        ss >> reg1 >> inc_dec >> amount1 >> if_ >> reg2 >> cond >> amount2;

        bool process = false;
        if (cond == ">")
        {
            process = registers[reg2] > amount2;
        }
        else if (cond == "<")
        {
            process = registers[reg2] < amount2;
        }
        else if (cond == "==")
        {
            process = registers[reg2] == amount2;
        }
        else if (cond == "<=")
        {
            process = registers[reg2] <= amount2;
        }
        else if (cond == ">=")
        {
            process = registers[reg2] >= amount2;
        }
        else if (cond == "!=")
        {
            process = registers[reg2] != amount2;
        }
        else
        {
            assert(false);
        }

        if (process)
        {
            if (inc_dec == "inc")
            {
                registers[reg1] += amount1;
            }
            else
            {
                registers[reg1] -= amount1;
            }

            if (registers[reg1] > part2)
                part2 = registers[reg1];
        }
    }

    auto max = INT_MIN;
    for (auto item : registers)
    {
        if (item.second > max)
            max = item.second;
    }

    return 0;
}
//7296, 8186