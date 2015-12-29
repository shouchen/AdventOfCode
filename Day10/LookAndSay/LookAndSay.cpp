// LookAndSay.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

std::string ProcessString(const char *str)
{
    std::ostringstream out;

    while (*str)
    {
        auto prevChar = *str;
        auto count = 0U;

        while (*str == prevChar)
            str++, count++;

        out << count << prevChar;
    }

    return out.str();
}

void _tmain(int argc, _TCHAR* argv[])
{
    std::string str = "3113322113";

    for (int i = 0; i < 40; i++)
        str = ProcessString(str.c_str());

    auto part1 = str.length();
    std::cout << "part one: " << str.length() << std::endl;
    assert(part1 == 329356);

    for (int i = 0; i < 10; i++)
        str = ProcessString(str.c_str());

    auto part2 = str.length();
    std::cout << "part2: " << str.length() << std::endl;
    assert(part2 == 4666278);
}
