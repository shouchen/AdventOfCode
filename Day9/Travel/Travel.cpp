// Travel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

std::map<std::string, std::map<std::string, unsigned>> distances;

void ReadInputFile(const std::string filename)
{
    std::ifstream f(filename);
    for (std::string line; getline(f, line);)
    {
        auto to = line.find(" to ");
        auto equals = line.find(" = ");

        auto origin = line.substr(0, to);
        auto destination = line.substr(to + 4, equals - to - 4);
        auto distance = atoi(line.substr(equals + 3).c_str());

        distances[origin][destination] = distances[destination][origin] = distance;
    }

    f.close();
}

void FindMinimumAndMaximum(unsigned &minimum, unsigned &maximum)
{
    minimum = UINT_MAX, maximum = 0;

    std::vector<std::string> cities;
    for (auto curr = distances.begin(); curr != distances.end(); curr++)
        cities.push_back(curr->first);

    do
    {
        auto distance = 0U;

        for (auto i = 0U; i < cities.size() - 1; i++)
            distance += distances[cities[i]][cities[i + 1]];

        minimum = std::min(minimum, distance);
        maximum = std::max(maximum, distance);
    } while (next_permutation(cities.begin(), cities.end()));
}

void _tmain(int argc, _TCHAR* argv[])
{
    ReadInputFile("Input.txt");

    auto minimum = 0U, maximum = 0U;
    FindMinimumAndMaximum(minimum, maximum);

    std::cout << "minimum = " << minimum << std::endl;
    std::cout << "maximum = " << maximum << std::endl;
}
