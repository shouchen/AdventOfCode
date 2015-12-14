// Happiness.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

std::map<std::string, std::map<std::string, unsigned>> distances;

// Foo would gain/lose <n> happiness point(s) by sitting next to bar.
void ReadInputFile(const std::string filename)
{
    std::ifstream f(filename);
    std::string word;

    for (std::string line; getline(f, line);)
    {
        auto endName1 = line.find(" would ");
        auto gainOrLose = line.substr(endName1 + 7, 4);

        auto origin = line.substr(0, to);
        auto destination = line.substr(to + 4, equals - to - 4);
        auto distance = atoi(line.substr(equals + 3).c_str());

        distances[origin][destination] = distances[destination][origin] = distance;
    }

    f.close();
}

int EvaluateHappiness()
{
    return 0;
}

void _tmain(int argc, _TCHAR *argv[])
{
    ReadInputFile("Input.txt");

    // todo:  do perms
    // Evaluate happiness and save best
}
