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
    unsigned literal = 0, actual = 0;

    // skip over opening quote, add one to literal but not actual
    line++, literal++;

    while (*line != '"')
    {
        auto literalRun = 1;

        if (*line == '\\')
        {
            if (line[1] == '\\' || line[1] == '"')
                literalRun = 2;
            else if (line[1] == 'x' && isxdigit(line[2]) && isxdigit(line[3]))
                literalRun = 4;
        }

        actual++;
        line += literalRun;
        literal += literalRun;
    }

    literal++; // for close quote
    return literal - actual;
}

unsigned ProcessLine2(const char *line)
{
    // 2 is for open/close quotes
    unsigned literal = 2, actual = 0;

    while (*line)
    {
        // these result in one additional literal character
        if (*line == '\\' || *line == '"')
            literal++;

        line++, actual++, literal++;
    }

    return literal - actual;
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
