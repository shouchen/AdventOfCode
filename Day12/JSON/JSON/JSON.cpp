// JSON.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cassert>
#include "JsonParser.h"

using namespace std;

/*
- [1, 2, 3] and { "a":2, "b" : 4 } both have a sum of 6.
- [[[3]]] and { "a":{"b":4}, "c" : -1 } both have a sum of 3.
- {"a":[-1, 1]} and [-1, { "a":1 }] both have a sum of 0.
- [] and{} both have a sum of 0.

object, array, value, string, number
http://json.org/
*/

string ExciseRedObjects(string str)
{
    // Find :"red"
    auto red = 0;
    while ((red = str.find(":\"red\"")) > 0)
    {
        // Back up to prev open brace
        unsigned depth = 0;

        red--;
        for (;;)
        {
            auto c = str[red];

            if (depth == 0 && c == '{')
                break;

            if (c == '}')
                depth++;
            if (c == '{')
                depth--;

            red--;
        }

        auto openBrace = red;

        // Find corresponding close brace
        depth = 0; // already zero?

        red++;
        for (;;)
        {
            auto c = str[red];

            if (depth == 0 && c == '}')
                break;

            if (c == '{')
                depth++;
            if (c == '}')
                depth--;

            red++;
        }

        auto closeBrace = red;

        // Cut out everything between braces
        auto temp1 = str.substr(0, openBrace + 1);
        auto temp2 = str.substr(closeBrace);

        str = temp1 + temp2;
    }

    return str;
}

unsigned CountNumbers(const string &line)
{
    unsigned total = 0;

    auto numberStart = line.end();

    for (auto curr = line.begin(); curr != line.end(); curr++)
    {
        if (isdigit(*curr) || *curr == '-')
        {
            if (numberStart == line.end())
                numberStart = curr;
        }
        else
        {
            if (numberStart != line.end())
            {
                auto number = atoi(std::string(numberStart, curr).c_str());
                total += number;
                numberStart = line.end();
            }
        }
    }

    return total;
}

void _tmain(int argc, _TCHAR *argv[])
{
    std::ifstream f("Input.txt");
    string line;
    if (!(f >> line)) return;

    auto total = CountNumbers(line);
    assert(total == 156366);
    std::cout << "part one: " << total << std::endl;
        
    total = CountNumbers(ExciseRedObjects(line));
    assert(total == 96852);
    std::cout << "part two: " << total << std::endl;
}
