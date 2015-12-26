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

        // Also look at steps
        return false;
    }
};

priority_queue<State, vector<State>, MyComparison> queue;

unsigned Solve(State state)
{
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
                State newState{ state.formula.substr(0, pos) + replacement.to + state.formula.substr(pos + replacement.from.length()), state.stepsSoFar + 1 };
                ::queue.push(newState);
                break;
            }
        }
    }
}

void _tmain(int argc, _TCHAR *argv[])
{
    ifstream f("Input.txt");
    BuildReplacements(f);

    string med;
    f >> med;

    State state{ med, 0 };
    ::queue.push(state);
    unsigned steps = Solve(state);
    cout << "part one: " << steps << endl;
    assert(steps == 212);
}
