// JSON.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>

void _tmain(int argc, _TCHAR *argv[])
{
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