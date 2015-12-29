// Reindeer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <cassert>

struct Reindeer
{
    void Move(unsigned whichSecond)
    {
        if ((whichSecond % (endurance + rest)) < endurance)
            distance += speed;
    }

    const unsigned speed, endurance, rest;
    unsigned distance, points;
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

        reindeer.push_back(Reindeer{ speed, endurance, rest, 0, 0 });
    }
}

void Solve(unsigned seconds, unsigned &maxDistance, unsigned &maxPoints)
{
    maxDistance = maxPoints = 0U;

    for (auto i = 0U; i < seconds; i++)
    {
        // Advance all the reindeer for this second
        for (auto &r : reindeer)
        {
            r.Move(i);
            maxDistance = std::max(maxDistance, r.distance);
        }

        // Award points to front-runner(s)
        for (auto &r : reindeer)
            if (r.distance == maxDistance)
                r.points++;
    }

    // Find max points
    for (auto &r : reindeer)
        maxPoints = std::max(maxPoints, r.points);
}

void _tmain(int argc, _TCHAR *argv[])
{
    ReadInputFile("Input.txt");

    auto maxDistance = 0U, maxPoints = 0U;
    Solve(2503, maxDistance, maxPoints);

    std::cout << "part one: " << maxDistance << std::endl;
    assert(maxDistance == 2696);

    std::cout << "part two: " << maxPoints << std::endl;
    assert(maxPoints == 1084);
}
