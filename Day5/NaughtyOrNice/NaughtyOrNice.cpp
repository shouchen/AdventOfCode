// NaughtyOrNice.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

bool IsNice1(const string &input)
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

bool IsNice2(const string &input)
{
    bool hasRepeatedPair = false;
    bool hasRepeatedWithSpaceBetween = false;

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

    ifstream f;
    f.open("input.txt");

    while (f.good())
    {
        string word;
        f >> word;

        if (IsNice1(word)) count1++;
        if (IsNice2(word)) count2++;
    }

    f.close();

    cout << "part one: " << count1 << endl;
    cout << "part two: " << count2 << endl;
}
