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

set<string> GenerateNewStringsByApplyingReplacement(const string &original, const string &from, const vector<string> &to)
{
    set<string> strings;
    auto place = -1;

    for (;;)
    {
        place = original.find(from, place + 1);
        if (place == string::npos) break;

        for (auto curr2 = to.begin(); curr2 != to.end(); curr2++)
        {
            auto newLine = original.substr(0, place) + *curr2 + original.substr(place + from.length());
            strings.insert(newLine);
        }
    }

    return strings;
}

unsigned CountDistinctPart1(const string &line)
{
    set<string> newLines;

    for (auto curr = replacements.begin(); curr != replacements.end(); curr++)
    {
        auto newStrings = GenerateNewStringsByApplyingReplacement(line, curr->first, curr->second);

        for (auto s : newStrings)
            newLines.insert(s);
    }

    return newLines.size();
}

void PartTwoAddStep(set<string> &all)
{
    set<string> next;

    for (auto currString = all.begin(); currString != all.end(); currString++)
    {
        for (auto curr = replacements.begin(); curr != replacements.end(); curr++)
        {
            auto newStrings = GenerateNewStringsByApplyingReplacement(*currString, curr->first, curr->second);

            for (auto s : newStrings)
                next.insert(s);
        }
    }

    all = next;
}

void _tmain(int argc, _TCHAR *argv[])
{
    ifstream f("Input.txt");
    BuildReplacements(f);

    string medicine;
    f >> medicine;

    unsigned part1 = CountDistinctPart1(medicine);
    assert(part1 == 535);
    cout << "part one: " << part1 << endl;
}
