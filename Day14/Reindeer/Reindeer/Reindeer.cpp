// Reindeer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <regex>
#include <cassert>

struct Reindeer
{
    unsigned speed;
    unsigned endurance;
    unsigned rest;
};

std::vector<Reindeer> reindeer;

void ReadInputFile(const std::string filename)
{
    std::smatch sm;
    std::regex e(".* can fly (\\d*) km/s for (\\d*) seconds+, but then must rest for (\\d*) seconds.");

    std::ifstream f(filename);

    for (std::string line; getline(f, line);)
    {
        std::regex_match(line, sm, e);

        assert(sm.size() == 4);

        auto speed = atoi(sm[1].str().c_str());
        auto endurance = atoi(sm[2].str().c_str());
        auto rest = atoi(sm[3].str().c_str());

        reindeer.push_back(Reindeer { speed, endurance, rest });
    }

    f.close();
}

unsigned GetDistance(const Reindeer &reindeer, unsigned seconds)
{
    // Calculate the number of complete run-rest cycles, and how far and how long it took.
    auto cycleDuration = reindeer.endurance + reindeer.rest;
    auto cycles = seconds / cycleDuration;
    auto baseTime = cycles * cycleDuration;
    auto baseDistance = cycles * reindeer.speed * reindeer.endurance;

    // Figure out how far the reindeer gets in the time remaining after the cycles.
    auto remainingTime = seconds - baseTime;
    auto remainingTimeToRun = std::min(remainingTime, reindeer.endurance);
    auto additionalDistance = remainingTimeToRun * reindeer.speed;

    return baseDistance + additionalDistance;
}

int SolvePuzzle(unsigned seconds)
{
    unsigned maxDistance = 0;

    for (auto &r : reindeer)
    {
        unsigned dist = GetDistance(r, seconds);
        maxDistance = std::max(maxDistance, dist);
    }

    return maxDistance;
}

void _tmain(int argc, _TCHAR *argv[])
{
    ReadInputFile("Input.txt");

    std::cout << SolvePuzzle(2503) << std::endl;
}
