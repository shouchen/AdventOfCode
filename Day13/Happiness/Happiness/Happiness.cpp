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

void ReadInputFile(const std::string filename)
{
    std::ifstream f(filename);

    for (std::string line; getline(f, line);)
    {
        std::smatch sm;
        std::regex e("(.*) would ([a-z]*) (\\d*) happiness units+ by sitting next to (.*).");
        std::regex_match(line, sm, e);

        assert(sm.size() == 5);

        std::string person1 = sm[1];
        std::string person2 = sm[4];
        int happiness = ((sm[2] == "gain") ? 1 : -1) * atoi(sm[3].str().c_str());

        happinessTable[person1][person2] = happiness;
    }

    f.close();
}

int GetHappiness(const std::string &person1, const std::string &person2)
{
    if (person1 == "" || person2 == "") return 0;
    return happinessTable[person1][person2];
}

int EvaluateHappiness(const std::vector<std::string> &people)
{
    int overallHappiness = 0;

    for (auto i = 0U; i < people.size(); i++)
    {
        auto leftIndex = ((i == 0) ? people.size() : i) - 1;
        auto rightIndex = (i == people.size() - 1) ? 0 : (i + 1);

        overallHappiness += happinessTable[people[i]][people[leftIndex]];
        overallHappiness += happinessTable[people[i]][people[rightIndex]];
    }

    return overallHappiness;
}

int FindHappiest1()
{
    std::vector<std::string> people;
    for (auto curr = happinessTable.begin(); curr != happinessTable.end(); curr++)
        people.push_back(curr->first);

    auto maxHappiness = INT_MIN;

    do
    {
        int happiness = EvaluateHappiness(people);
        if (happiness > maxHappiness)
            maxHappiness = happiness;
    } while (next_permutation(people.begin(), people.end()));

    return maxHappiness;
}

int FindHappiest2()
{
    std::vector<std::string> people;
    people.push_back(""); // me
    for (auto curr = happinessTable.begin(); curr != happinessTable.end(); curr++)
        people.push_back(curr->first);

    auto maxHappiness = INT_MIN;

    do
    {
        int happiness = EvaluateHappiness(people);
        if (happiness > maxHappiness)
            maxHappiness = happiness;
    } while (next_permutation(people.begin(), people.end()));

    return maxHappiness;
}

void _tmain(int argc, _TCHAR *argv[])
{
    ReadInputFile("Input.txt");

    std::vector<std::string> people;
    for (auto curr = happinessTable.begin(); curr != happinessTable.end(); curr++)
        people.push_back(curr->first);

    auto maxHappiness = INT_MIN;

    do
    {
        int happiness = EvaluateHappiness(people);
        if (happiness > maxHappiness)
            maxHappiness = happiness;
    } while (next_permutation(people.begin(), people.end()));

    std::cout << "part one: " << FindHappiest1() << std::endl;
    std::cout << "part two: " << FindHappiest2() << std::endl;
}
