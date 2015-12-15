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

/*
- [1, 2, 3] and { "a":2, "b" : 4 } both have a sum of 6.
- [[[3]]] and { "a":{"b":4}, "c" : -1 } both have a sum of 3.
- {"a":[-1, 1]} and [-1, { "a":1 }] both have a sum of 0.
- [] and{} both have a sum of 0.

object, array, value, string, number
http://json.org/
*/


void _tmain(int argc, _TCHAR *argv[])
{
    const char *value = "  [1, 2, 3]";
    JsonValue *val = ParseValue(value);

    //////////////////
    unsigned total = 0;

    std::ifstream f("Input.txt");
    for (std::string line; getline(f, line);)
    {
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
    }

    f.close();

    std::cout << total << std::endl;
}