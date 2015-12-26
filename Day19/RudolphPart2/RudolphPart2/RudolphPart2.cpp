// RudolphPart2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <cassert>

using namespace std;

struct Replacement
{
    string from;
    string to;
};

vector<Replacement> replacements;

void BuildReplacements(ifstream &f)
{
    for (string line; getline(f, line);)
    {
        if (line.length() == 0)
            break;

        auto arrow = line.find(" => ");
        auto from = line.substr(0, arrow);
        auto to = line.substr(arrow + 4);

        replacements.push_back(Replacement{ to, from });
    }
}

struct State
{
    string formula;
    unsigned stepsSoFar;
};

struct MyComparison
{
    bool operator()(const State &lhs, const State &rhs) const
    {
        return lhs.formula.length() > rhs.formula.length();

        // Also look at steps?
        return false;
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

        cout << state.formula.length() << endl;

        // Find a replacement and make it
        for (auto &replacement : replacements)
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
}

void _tmain(int argc, _TCHAR *argv[])
{
    ifstream f("Input.txt");
    BuildReplacements(f);

    string medicine;
    f >> medicine;

    unsigned steps = Solve(medicine);
    cout << "part two: " << steps << endl;
    assert(steps == 212);
}
