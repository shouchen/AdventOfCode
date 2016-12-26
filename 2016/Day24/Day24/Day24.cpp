#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <array>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <ctime>
#include <cassert>

using namespace std::string_literals;
using DigitDistArray = std::array<unsigned, 10>;

struct Coords
{
    unsigned row, col;
};

std::vector<std::string> maze;
std::vector<std::vector<DigitDistArray>> distFromDigit;
std::vector<Coords> digitLocation;

void ReadMaze(const std::string filename)
{
    std::ifstream f(filename);
    std::string line;

    while (std::getline(f, line))
    {
        maze.push_back(line);

        auto row = distFromDigit.size();
        distFromDigit.push_back(std::vector<DigitDistArray>());

        for (auto col = 0U; col < line.length(); col++)
        {
            distFromDigit[row].push_back(DigitDistArray
                {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX,
                 UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX } );

            auto mazeCell = maze[row][col];
            if (isdigit(mazeCell))
            {
                auto digit = mazeCell - '0';

                if (digitLocation.size() < digit + 1U)
                    digitLocation.resize(digit + 1U);

                digitLocation[digit] = { row, col };
            }
        }
    }
}

inline void TryUpdateDistance(std::queue<Coords> &q, const Coords &coords, unsigned digit, unsigned newDistance)
{
    auto &oldDistance = distFromDigit[coords.row][coords.col][digit];

    if (maze[coords.row][coords.col] != '#' && newDistance < oldDistance)
    {
        oldDistance = newDistance;
        q.push(coords);
    }
}

void ComputeDistancesFromTargets()
{
    for (auto digit = 0U; digit < digitLocation.size(); digit++)
    {
        std::queue<Coords> q;
        TryUpdateDistance(q, digitLocation[digit], digit, 0);

        while (!q.empty())
        {
            auto row = q.front().row, col = q.front().col;
            auto newDistance = distFromDigit[row][col][digit] + 1;

            TryUpdateDistance(q, Coords{ row - 1, col }, digit, newDistance);
            TryUpdateDistance(q, Coords{ row + 1, col }, digit, newDistance);
            TryUpdateDistance(q, Coords{ row, col - 1 }, digit, newDistance);
            TryUpdateDistance(q, Coords{ row, col + 1 }, digit, newDistance);

            q.pop();
        }
    }
}

inline unsigned GetDistanceBetweenDigits(unsigned fromDigit, unsigned toDigit)
{
    return distFromDigit[digitLocation[fromDigit].row][digitLocation[fromDigit].col][toDigit];
}

inline unsigned GetPathDistance(const std::string &path)
{
    unsigned cost = 0;
    for (auto i = 0U; i < path.length() - 1; i++)
        cost += GetDistanceBetweenDigits(path[i] - '0', path[i + 1] - '0');

    return cost;
}

void Solve(const std::string &filename, unsigned &part1, unsigned &part2)
{
    ReadMaze(filename);
    ComputeDistancesFromTargets();

    part1 = part2 = UINT_MAX;
    auto perm = "0123456789"s.substr(0, digitLocation.size());

    do
    {
        auto cost1 = GetPathDistance(perm);
        auto cost2 = cost1 + GetDistanceBetweenDigits(perm[perm.length() - 1] - '0', 0);

        part1 = std::min(cost1, part1);
        part2 = std::min(cost2, part2);
    } while (std::next_permutation(perm.begin() + 1, perm.end()));
}

int _tmain(int argc, _TCHAR *argv[])
{
    double startTime = clock();

    auto part1 = UINT_MAX, part2 = UINT_MAX;
    Solve("input.txt", part1, part2);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;

    assert(part1 == 502);
    assert(part2 == 724);
    return 0;
}
