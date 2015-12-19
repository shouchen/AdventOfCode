// Rudolph.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <cassert>

using namespace std;

map<string, vector<string>> replacements;

void BuildReplacements(ifstream &f)
{
    for (string line; getline(f, line);)
    {
        if (line.length() == 0)
            break;

        auto arrow = line.find(" => ");
        auto from = line.substr(0, arrow);
        auto to = line.substr(arrow + 4);

        replacements[from].push_back(to);
    }
}

unsigned CountDistinctReplacements(const string &line)
{
    set<string> newLines;

    cout << line << endl << endl;
    for (auto curr = replacements.begin(); curr != replacements.end(); curr++)
    {
        auto place = -1;
        for (;;)
        {
            place = line.find(curr->first, place + 1);
            if (place == string::npos) break;

            cout << "place = " << place << endl;

            for (auto curr2 = curr->second.begin(); curr2 != curr->second.end(); curr2++)
            {
                auto newLine = line.substr(0, place) + *curr2 + line.substr(place + curr->first.length());
                newLines.insert(newLine);
            }
        }
    }

    return newLines.size();
}

unsigned PartTwo(const string &medicine)
{
    return 0;
}

void _tmain(int argc, _TCHAR *argv[])
{
    ifstream f("Input.txt");
    BuildReplacements(f);

    string medicine;
    f >> medicine;

    unsigned part1 = CountDistinctReplacements(medicine);
    assert(part1 == 535);
    cout << "part one: " << part1 << endl;

    unsigned part2 = PartTwo(medicine);
    //assert(part2 == 535);
    cout << "part two: " << part2 << endl;
}
