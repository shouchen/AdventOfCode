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

struct Replacement2
{
    string from, to;
};

struct State
{
    string formula;
    unsigned stepsSoFar;
};

map<string, vector<string>> replacements1;
vector<Replacement2> replacements2;

void BuildReplacements(ifstream &f)
{
    for (string line; getline(f, line);)
    {
        if (line.length() == 0)
            break;

        auto arrow = line.find(" => ");
        replacements1[line.substr(0, arrow)].push_back(line.substr(arrow + 4));
        replacements2.push_back(Replacement2{ line.substr(arrow + 4), line.substr(0, arrow) });
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

    for (auto curr = replacements1.begin(); curr != replacements1.end(); curr++)
    {
        auto newStrings = GenerateNewStringsByApplyingReplacement(line, curr->first, curr->second);

        for (auto s : newStrings)
            newLines.insert(s);
    }

    return newLines.size();
}

struct MyComparison
{
    bool operator()(const State &lhs, const State &rhs) const
    {
        return lhs.formula.length() > rhs.formula.length();
    }
};

priority_queue<State, vector<State>, MyComparison> queue;

unsigned Solve(string medicine)
{
    State state{ medicine, 0 };
    ::queue.push(state);

    while (!::queue.empty())
    {
        State state = ::queue.top();
        ::queue.pop();

        if (state.formula == "e")
            return state.stepsSoFar;

        // Find a replacement and make it
        for (auto &replacement : replacements2)
        {
            int pos = state.formula.find(replacement.from);
            if (pos > -1)
            {
                auto newFormula = state.formula.substr(0, pos) + replacement.to + state.formula.substr(pos + replacement.from.length());
                ::queue.push(State{ newFormula, state.stepsSoFar + 1 });
                break;
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
    assert(part1 == 535);
    cout << "part one: " << part1 << endl;

    unsigned part2 = Solve(medicine);
    cout << "part two: " << part2 << endl;
    assert(part2 == 212);
}
