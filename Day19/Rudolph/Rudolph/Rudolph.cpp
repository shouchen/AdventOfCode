// Rudolph.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>

using namespace std;

map<string, vector<string>> replacements;

void AddReplacement(string line)
{
    auto arrow = line.find(" => ");
    auto from = line.substr(0, arrow);
    auto to = line.substr(arrow + 4);

    replacements[from].push_back(to);
}

unsigned CountDistinctReplacements(string line)
{
    set<string> newLines;

    cout << line << endl << endl;
    for (auto curr = replacements.begin(); curr != replacements.end(); curr++)
    {
        cout << endl << "FOR REPLACEMENT \"" << curr->first << "\":" << endl;

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
                //cout << newLine << endl;
            }
        }
    }

    return newLines.size();
}

void _tmain(int argc, _TCHAR *argv[])
{
    ifstream f("Input.txt");
    string line;

    for (string line; getline(f, line);)
    {
        if (line.length() == 0)
            break;

        AddReplacement(line);
    }

    f >> line;
    unsigned answer = CountDistinctReplacements(line); // 638 was too high
    cout << "part one: " << answer << endl;
}
