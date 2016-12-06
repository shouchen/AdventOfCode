// Escaping.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>

unsigned ProcessLinePart1(const char *line)
{
    // literal start with two more for open + close quotes
    auto totalDiff = 2U;

    // skip over opening quote
    assert(*line == '"');
    line++;

    for (auto curr = line; *curr != '"'; curr++)
    {
        if (*curr == '\\')
        {
            auto diff = 0U;

            if (curr[1] == '\\' || curr[1] == '"')
                diff = 1;
            else if (curr[1] == 'x' && isxdigit(curr[2]) && isxdigit(curr[3]))
                diff = 3;

            curr += diff;
            totalDiff += diff;
        }
    }

    return totalDiff;
}

unsigned ProcessLinePart2(const char *line)
{
    // 2 is for open/close quotes
    auto totalDiff = 2U;

    while (*line)
    {
        // these result in one additional literal character
        if (*line == '\\' || *line == '"')
            totalDiff++;

        line++;
    }

    return totalDiff;
}

void _tmain(int argc, _TCHAR* argv[])
{
    std::ifstream f("Input.txt");

    auto total1 = 0U, total2 = 0U;
    std::string line;

    while (f >> line)
    {
        total1 += ProcessLinePart1(line.c_str());
        total2 += ProcessLinePart2(line.c_str());
    }

    std::cout << "part one = " << total1 << std::endl;
    assert(total1 == 1342);

    std::cout << "part two = " << total2 << std::endl;
    assert(total2 == 2074);
}
