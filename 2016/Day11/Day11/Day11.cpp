#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include <cassert>

enum Name : char
{
    Polonium_M = 0x00,
    Polonium_G = 0x01,
    Thulium_M = 0x02,
    Thulium_G = 0x03,
    Promethium_M = 0x04,
    Promethium_G = 0x05,
    Ruthenium_M = 0x06,
    Ruthenium_G = 0x07,
    Cobalt_M = 0x08,
    Cobalt_G = 0x09,

    Elerium_M = 0x0A,
    Elerium_G = 0x0B,
    Dilithium_M = 0x0C,
    Dilithium_G = 0x0D,
    NumNames
};

const char *GetName(Name name)
{
    switch (name)
    {
    case Polonium_M: return "Polonium_M";
    case Polonium_G: return "Polonium_G";
    case Thulium_M: return "Thulium_M";
    case Thulium_G: return "Thulium_G";
    case Promethium_M: return "Promethium_M";
    case Promethium_G: return "Promethium_G";
    case Ruthenium_M: return "Ruthenium_M";
    case Ruthenium_G: return "Ruthenium_G";
    case Cobalt_M: return "Cobalt_M";
    case Cobalt_G: return "Cobalt_G";

    case Elerium_M: return "Elerium_M";
    case Elerium_G: return "Elerium_G";
    case Dilithium_M: return "Dilithium_M";
    case Dilithium_G: return "Dilithium_G";
    }

    assert(false);
    return "<unknown>";
}

struct Thing
{
    Name name;
    char floor;
};

int elevator = 1;

Thing input[NumNames] =
{
    { Polonium_M, 2 },
    { Promethium_M, 2 },
    { Polonium_G , 1 },
    { Thulium_G , 1 },
    { Thulium_M , 1 },
    { Promethium_G , 1 },
    { Ruthenium_G , 1 },
    { Ruthenium_M , 1 },
    { Cobalt_G , 1 },
    { Cobalt_M , 1 },

    { Elerium_G , 1 },
    { Elerium_M , 1 },
    { Dilithium_G , 1 },
    { Dilithium_M , 1 },
};

struct State
{
    State(Thing input[NumNames]) : elevator(1), numMoves(0)
        { for (int i = 0; i < NumNames; i++) this->input[i] = input[i]; }
    unsigned Hash()
    {
        unsigned val = elevator - 1;
        for (int i = 0; i < NumNames; i++)
        {
            val = (val << 2) | (input[i].floor - 1);
        }
        return val;
    }

    Thing input[NumNames];
    int elevator;
    int numMoves;
};

std::set<unsigned> seen;
std::queue<State> queue;

void Dump(const State &state)
{
    for (int f = 4; f > 0; f--)
    {
        std::cout << ((state.elevator == f) ? 'E' : ' ') << f << " ";
        for (auto thing : state.input)
        {
            if (thing.floor == f) std::cout << GetName(thing.name) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

bool ValidConfig(const State &state)
{
    // See if any chips will "fry"; loop through all chips.
    for (auto i = 0; i < NumNames; i++)
    {
        if ((state.input[i].name & 0x01) == 0) // a chip
        {
            bool withCorrespondingGenerator = false, withDifferentGenerator = false;

            // Look through all generators on the same floor
            for (auto j = 0; j < NumNames; j++)
            {
                if (state.input[i].floor == state.input[j].floor)
                {
                    if ((state.input[i].name ^ state.input[j].name) == 1)
                        withCorrespondingGenerator = true;
                    else if (state.input[j].name & 0x01)
                        withDifferentGenerator = true;
                }
            }

            if (withDifferentGenerator && !withCorrespondingGenerator)
                return false;
        }
    }

    return true;
}

bool IsSolved(const State &state)
{
    for (auto i = 0; i < sizeof(state.input) / sizeof(state.input[0]); i++)
        if (state.input[i].floor != 4) return false;

    return true;
}

int best_solution = INT_MAX;

bool Solve()
{
    if (queue.empty()) return false;

    State state = queue.front();
    queue.pop();

    if (state.numMoves >= best_solution)
        return true;

    if (!ValidConfig(state))
        return true;

    if (IsSolved(state))
    {
        if (state.numMoves < best_solution)
            best_solution = state.numMoves;

        return true;
    }

    auto hash = state.Hash();
    if (seen.find(hash) != seen.end())
        return true;

    seen.insert(hash);

    // Elevator up
    if (state.elevator < 4)
    {
        // With 1 thing
        for (auto i = 0; i < NumNames; i++)
        {
            if (state.input[i].floor == state.elevator)
            {
                State newState = state;
                newState.numMoves++;
                newState.input[i].floor++;
                newState.elevator++;

                queue.push(newState);

                // With 2 things
                for (auto j = i + 1; j < NumNames; j++)
                {
                    if (state.input[j].floor == state.elevator)
                    {
                        State newState2 = newState;
                        newState2.input[j].floor++;

                        queue.push(newState2);
                    }
                }
            }
        }
    }

    // Elevator down
    if (state.elevator > 1)
    {
        // With 1 thing
        for (auto i = 0; i < NumNames; i++)
        {
            if (state.input[i].floor == state.elevator)
            {
                State newState = state;
                newState.numMoves++;
                newState.input[i].floor--;
                newState.elevator--;

                queue.push(newState);

                // With 2 things
                for (auto j = i + 1; j < NumNames; j++)
                {
                    if (state.input[j].floor == state.elevator)
                    {
                        State newState2 = newState;
                        newState2.input[j].floor--;

                        queue.push(newState2);
                    }
                }
            }
        }
    }

    return true;
}

int main()
{
    State state(input);
    queue.push(state);

    while(Solve());

    std::cout << "Part One: " << best_solution << std::endl;
    //std::cout << "Part Two: " << answer2 << std::endl;

    //assert(best_solution == 47);
    //assert(best_solution == 71);
    return 0;
}
