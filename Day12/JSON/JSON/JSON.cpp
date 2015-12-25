// JSON.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>


unsigned GetStartOfObject(const std::string &str, unsigned pos)
{
    auto depth = 0;

    for (;;)
    {
        switch (str[--pos])
        {
        case '{': if (depth-- == 0) return pos; break;
        case '}': depth++; break;
        }
    }
}

unsigned GetEndOfObject(const std::string &str, unsigned pos)
{
    auto depth = 0;

    for (;;)
    {
        switch (str[++pos])
        {
        case '}': if (depth-- == 0) return pos; break;
        case '{': depth++; break;
        }
    }
}

std::string ExciseRedObjects(std::string str)
{
    auto red = 0;
    while ((red = str.find(":\"red\"")) > 0)
    {
        // Cut out any entire object that has red in it
        auto openBrace = GetStartOfObject(str, red);
        auto closeBrace = GetEndOfObject(str, openBrace);

        str = str.substr(0, openBrace + 1) + str.substr(closeBrace);
    }

    return str;
}

unsigned CountNumbers(const std::string &line)
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
        else if (numberStart != line.end())
        {
            total += atoi(std::string(numberStart, curr).c_str());
            numberStart = line.end();
        }
    }

    return total;
}

void _tmain(int argc, _TCHAR *argv[])
{
    std::ifstream f("Input.txt");
    std::string line;
    if (!(f >> line)) return;

    auto total = CountNumbers(line);
    assert(total == 156366);
    std::cout << "part one: " << total << std::endl;
        
    total = CountNumbers(ExciseRedObjects(line));
    assert(total == 96852);
    std::cout << "part two: " << total << std::endl;
}
