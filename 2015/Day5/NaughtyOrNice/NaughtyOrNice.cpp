// NaughtyOrNice.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>

bool IsNice1(const std::string &input)
{
    auto numVowels = 0;
    auto hasDouble = false;
    auto previousLetter = '\0';

    for (auto curr = input.c_str(); *curr; curr++)
    {
        if (strncmp("ab", curr, 2) == 0 || strncmp("cd", curr, 2) == 0 ||
            strncmp("pq", curr, 2) == 0 || strncmp("xy", curr, 2) == 0)
        {
            return false;
        }

        if (strchr("aeiou", *curr)) numVowels++;

        if (*curr == previousLetter) hasDouble = true;

        previousLetter = *curr;
    }

    return hasDouble && (numVowels >= 3);
}

bool IsNice2(const std::string &input)
{
    auto hasRepeatedPair = false;
    auto hasRepeatedWithSpaceBetween = false;

    for (auto curr = input.c_str(); *curr; curr++)
    {
        // Rule 1: Repeated pair
        if (curr[1])
            for (const char *other = curr + 2; *other; other++)
                if (curr[0] == other[0] && curr[1] == other[1])
                    hasRepeatedPair = true;

        // Rule 2: Repeated letter with something in between
        if (curr[1] && curr[2] == curr[0])
            hasRepeatedWithSpaceBetween = true;

        if (hasRepeatedPair && hasRepeatedWithSpaceBetween) 
            return true;
    }

    return false;
}

void _tmain(int argc, _TCHAR *argv[])
{
    auto count1 = 0, count2 = 0;

    std::ifstream f;
    f.open("input.txt");

    while (f.good())
    {
        std::string word;
        f >> word;

        if (IsNice1(word)) count1++;
        if (IsNice2(word)) count2++;
    }

    std::cout << "part one: " << count1 << std::endl;
    assert(count1 == 238);

    std::cout << "part two: " << count2 << std::endl;
    assert(count2 == 69);
}
