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

enum Name
{
    Hydrogen_M = 0x00,
    Hydrogen_G = 0x01,
    Lithium_M = 0x02,
    Lithium_G = 0x03,
    NumNames
};

//enum Name
//{
//    Polonium_M = 0x00,
//    Polonium_G = 0x01,
//    Thulium_M = 0x02,
//    Thulium_G = 0x03,
//    Promethium_M = 0x04,
//    Promethium_G = 0x05,
//    Ruthenium_M = 0x06,
//    Ruthenium_G = 0x07,
//    Cobalt_M = 0x08,
//    Cobalt_G = 0x09,
//    NumNames
//};

const char *GetName(Name name)
{
    switch (name)
    {
    case Hydrogen_M: return "Hydrogen_M";
    case Hydrogen_G: return "Hydrogen_G";
    case Lithium_M: return "Lithium_M";
    case Lithium_G: return "Lithium_G";
    }

    assert(false);
    return "<unknown>";
}

//const char *GetName(Name name)
//{
//    switch (name)
//    {
//    case Polonium_M: return "Polonium_M";
//    case Polonium_G: return "Polonium_G";
//    case Thulium_M: return "Thulium_M";
//    case Thulium_G: return "Thulium_G";
//    case Promethium_M: return "Promethium_M";
//    case Promethium_G: return "Promethium_G";
//    case Ruthenium_M: return "Ruthenium_M";
//    case Ruthenium_G: return "Ruthenium_G";
//    case Cobalt_M: return "Cobalt_M";
//    case Cobalt_G: return "Cobalt_G";
//    }
//
//    assert(false);
//    return "<unknown>";
//}

struct Thing
{
    Name name;
    int floor;
};

int elevator = 1;
Thing input[NumNames] =
{
    { Lithium_G, 3 },
    { Hydrogen_G, 2 },
    { Hydrogen_M, 1 },
    { Lithium_M , 1 }
};

//Thing input[NumNames] =
//{
//    { Polonium_M, 2 },
//    { Promethium_M, 2 },
//    { Polonium_G , 1 },
//    { Thulium_G , 1 },
//    { Thulium_M , 1 },
//    { Promethium_G , 1 },
//    { Ruthenium_G , 1 },
//    { Ruthenium_M , 1 },
//    { Cobalt_G , 1 },
//    { Cobalt_M , 1 },
//};

struct State
{
    State(Thing input[NumNames]) : elevator(1), numMoves(0)
        { for (int i = 0; i < NumNames; i++) this->input[i] = input[i]; }
    Thing input[NumNames];
    int elevator;
//    std::string soFar;
    int numMoves;
};

std::set<std::string> seen;
std::queue<State> queue;

std::string Hash()
{
    std::string retval;
    retval.push_back('E');
    retval.push_back(elevator + '0');
    retval.push_back(' ');

    for (int e = 0; e < NumNames; e++)
    {
        for (auto i = 0; i < NumNames; i++)
        {
            if (input[i].name == e)
            {
                retval += GetName(input[i].name);
                retval.push_back('=');
                retval.push_back(input[i].floor + '0');
                retval.push_back(' ');
                break;
            }
        }
    }

    return retval;
}

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

int num_steps = 0;
int best_solution = INT_MAX;

bool Solve()
{
    if (queue.empty()) return false;

    State state = queue.front();
    queue.pop();

    //if (num_steps >= best_solution)
    //    return;

    //std::cout << "So far: " << soFar << std::endl;
    //if (state.soFar == "U=Hydrogen_M U=Hydrogen_G/Hydrogen_M D=Hydrogen_M ")
    //if (state.soFar == "U=Hydrogen_M U=Hydrogen_G/Hydrogen_M ")
    //{
    //    Dump(state);
    //}

    if (IsSolved(state))
    {
        std::cout << "SOLUTION in " << num_steps << std::endl;
        if (num_steps < best_solution)
        {
            best_solution = num_steps;
            return false;
        }
    }

    if (!ValidConfig(state))
    {
        return true;
    }

    //std::string hash = Hash();
    //if (seen.find(hash) != seen.end())
    //    return;

    //seen.insert(hash);

    // Elevator up
    if (state.elevator < 4)
    {
        // With 1 thing
        for (auto i = 0; i < NumNames; i++)
        {
            if (state.input[i].floor == state.elevator)
            {
                State newState = state;
                //newState.soFar += (std::string)"U=" + GetName(state.input[i].name) + " ";
                newState.numMoves++;
                newState.input[i].floor++;
                newState.elevator++;

                queue.push(newState);
            }
        }

        // With 2 things
        for (auto i = 0; i < NumNames; i++)
        {
            for (auto j = i + 1; j < NumNames; j++)
            {
                if (state.input[i].floor == state.elevator && state.input[j].floor == state.elevator)
                {
                    State newState = state;
                    //newState.soFar += (std::string)"U=" + GetName(state.input[i].name) + "/" + GetName(state.input[j].name) + " ";
                    newState.numMoves++;
                    newState.input[i].floor++;
                    newState.input[j].floor++;
                    newState.elevator++;

                    queue.push(newState);
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
                //newState.soFar += (std::string)"D=" + GetName(state.input[i].name) + " ";
                newState.numMoves++;
                newState.input[i].floor--;
                newState.elevator--;

                queue.push(newState);
            }
        }

        // With 2 things
        for (auto i = 0; i < NumNames; i++)
        {
            for (auto j = i + 1; j < NumNames; j++)
            {
                if (state.input[i].floor == state.elevator && state.input[j].floor == state.elevator)
                {
                    State newState = state;
                    //newState.soFar += (std::string)"D=" + GetName(state.input[i].name) + "/" + GetName(state.input[j].name) + " ";
                    newState.numMoves++;
                    newState.input[i].floor--;
                    newState.input[j].floor--;
                    newState.elevator--;

                    queue.push(newState);
                }
            }
        }
    }

    return true;
}

//void ReadFile(const std::string &filename)
//{
//    if (filename == "input-test.txt")
//    {
//        board[4].push_back 
//
//        //4 
//        //3 Lithium(A)-G
//        //2.Hydrogen(B)-G
//        //1 Hydrogen(B)-M Lithium(A)-M
//    }
//
//    //std::ifstream f(filename);
//    //std::string word;
//
//    //int floor = -1;
//    //while (f >> word)
//    //{
//    //    if (word.length() == 2 && word[0] == 'F' && isdigit(word[1]))
//    //        floor = word[1] - '0';
//    //    else if (word == "E")
//    //        elevator = floor;
//    //    else if (word != ".")
//    //        place[floor].push_back(word);
//    //}
//
//    //Dump();
//}

int main()
{
//    Dump();
//    ReadFile("input-test.txt");
    State state(input);
    queue.push(state);

    while(Solve());

    //auto answer1 = botThatCompares17And61;
    //auto answer2 = outputs[0] * outputs[1] * outputs[2];

    std::cout << "Part One: " << best_solution << std::endl;
    //std::cout << "Part Two: " << answer2 << std::endl;

    //assert(answer1 == 27);
    //assert(answer2 == 13727);
    return 0;
}
