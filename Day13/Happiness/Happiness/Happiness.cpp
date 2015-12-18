// Happiness.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <regex>
#include <cassert>

std::map<std::string, std::map<std::string, int>> happinessTable;
std::vector<std::string> people;

void ReadInputFile(const std::string filename)
{
    std::smatch sm;
    std::regex e("(.*) would ([a-z]*) (\\d*) happiness units+ by sitting next to (.*).");

    std::ifstream f(filename);

    for (std::string line; getline(f, line);)
    {
        std::regex_match(line, sm, e);

        assert(sm.size() == 5);

        auto person1 = sm[1], person2 = sm[4];
        auto amount = atoi(sm[3].str().c_str());

        happinessTable[person1][person2] = (sm[2] == "gain") ? amount : -amount;
    }
}

int GetHappinessOfPerson1NextToPerson2(const std::string &person1, const std::string &person2)
{
    return (person1 != "" && person2 != "") ? happinessTable[person1][person2] : 0;
}

int EvaluateHappinessOfPermutation(const std::vector<std::string> &people)
{
    int happiness = 0;

    for (auto i = 0U; i < people.size(); i++)
    {
        auto leftIndex = ((i == 0) ? people.size() : i) - 1;
        auto rightIndex = (i == people.size() - 1) ? 0 : (i + 1);

        happiness += GetHappinessOfPerson1NextToPerson2(people[i], people[leftIndex]);
        happiness += GetHappinessOfPerson1NextToPerson2(people[i], people[rightIndex]);
    }

    return happiness;
}

int FindHappiestPermutation()
{
    auto maxHappiness = INT_MIN;

    do
        maxHappiness = std::max(maxHappiness, EvaluateHappinessOfPermutation(people));
    while (next_permutation(people.begin(), people.end()));

    return maxHappiness;
}

void SeatAllPeopleAtTable()
{
    for (auto curr = happinessTable.begin(); curr != happinessTable.end(); curr++)
        people.push_back(curr->first);
}

void AddSelfToTable()
{
    people.push_back("");
}

void _tmain(int argc, _TCHAR *argv[])
{
    ReadInputFile("Input.txt");

    SeatAllPeopleAtTable();
    std::cout << "part one: " << FindHappiestPermutation() << std::endl;

    AddSelfToTable();
    std::cout << "part two: " << FindHappiestPermutation() << std::endl;
}
