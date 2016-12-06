// Rudolph.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <queue>
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
        replacements[line.substr(0, arrow)].push_back(line.substr(arrow + 4));
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

        for (auto curr = to.begin(); curr != to.end(); curr++)
        {
            auto newLine = original.substr(0, place) + *curr + original.substr(place + from.length());
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

unsigned GetStepsForPart2(string medicine)
{
    struct State
    {
        string formula;
        unsigned stepsSoFar;
    };

    struct ShortestFormulaComparator
    {
        bool operator()(const State &lhs, const State &rhs) const
        { return lhs.formula.length() > rhs.formula.length(); }
    };

    priority_queue<State, vector<State>, ShortestFormulaComparator> queue;

    // Start with the medicine formula and try to reduce it to "e".
    State state{ medicine, 0 };
    queue.push(state);

    while (!queue.empty())
    {
        State state = queue.top();
        queue.pop();

        if (state.formula == "e")
            return state.stepsSoFar;

        // Find a replacement and make it
        for (auto &replacement : replacements)
        {
            for (auto &to : replacement.second)
            {
                auto pos = state.formula.find(to);
                if (pos != -1)
                {
                    auto newFormula = state.formula.substr(0, pos) + replacement.first + state.formula.substr(pos + to.length());
                    queue.push(State{ newFormula, state.stepsSoFar + 1 });
                    break;
                }
            }
        }
    }

    return UINT_MAX;
}

void _tmain(int argc, _TCHAR *argv[])
{
    ifstream f("Input.txt");
    BuildReplacements(f);

    string medicine;
    f >> medicine;

    unsigned part1 = CountDistinctPart1(medicine);
    cout << "part one: " << part1 << endl;
    assert(part1 == 535);

    unsigned part2 = GetStepsForPart2(medicine);
    cout << "part two: " << part2 << endl;
    assert(part2 == 212);
}
