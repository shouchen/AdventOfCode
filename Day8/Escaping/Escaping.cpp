// Escaping.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

unsigned ProcessLine1(const char *line)
{
    // literal start with two more for open + close quotes
    unsigned totalDiff = 2;

    // skip over opening quote
    assert(*line == '"');
    line++;

    for (auto curr = line; *curr != '"'; curr++)
    {
        if (*curr == '\\')
        {
            unsigned diff = 0;

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

unsigned ProcessLine2(const char *line)
{
    // 2 is for open/close quotes
    unsigned totalDiff = 2;

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
    ifstream f("Input.txt");
    string line;
    unsigned total1 = 0, total2 = 0;

    while (f >> line)
    {
        total1 += ProcessLine1(line.c_str());
        total2 += ProcessLine2(line.c_str());
    }

    cout << "part one = " << total1 << endl;
    cout << "part two = " << total2 << endl;
}
