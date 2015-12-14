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

// Foo would gain/lose <n> happiness point(s) by sitting next to bar.
void ReadInputFile(const std::string filename)
{
    std::ifstream f(filename);
    std::string word;

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

int EvaluateHappiness(const std::vector<std::string> &people)
{
    //std::cout << "Evaluating: ";
    //for (auto person : people)
    //{
    //    std::cout << person << " ";
    //}
    //std::cout << std::endl;

    int overallHappiness = 0;

    for (auto i = 0U; i < people.size(); i++)
    {
        auto leftIndex = ((i == 0) ? people.size() : i) - 1;
        auto rightIndex = (i == people.size() - 1) ? 0 : (i + 1);

        overallHappiness += happinessTable[people[i]][people[leftIndex]];
        overallHappiness += happinessTable[people[i]][people[rightIndex]];
        //std::cout << happinessTable[people[i]][people[leftIndex]] << "," << happinessTable[people[i]][people[rightIndex]];
    }

    //std::cout << "returning " << overallHappiness << std::endl;
    return overallHappiness;
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

    std::cout << maxHappiness << std::endl;
}
