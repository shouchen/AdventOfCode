#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <ctime>
#include <cassert>

using namespace std::string_literals;

// TODO: Read these programmatically from file
#define ROWS 39
#define COLS 183
#define DIGITS 8

struct Coords
{
    unsigned row, col;
};

std::vector<std::string> maze;
unsigned dists[ROWS][COLS][DIGITS];
Coords targets[DIGITS];

void ReadMaze(const std::string filename)
{
    std::ifstream f(filename);
    std::string line;

    while (std::getline(f, line))
        maze.push_back(line);
}

void InitializeMetrics()
{
    for (auto row = 0U; row < ROWS; row++)
    {
        for (auto col = 0U; col < COLS; col++)
        {
            for (auto digit = 0U; digit < DIGITS; digit++)
                dists[row][col][digit] = UINT_MAX;

            if (isdigit(maze[row][col]))
            {
                auto target = maze[row][col] - '0';
                dists[row][col][target] = 0;
                targets[target] = { row, col };
            }
        }
    }
}

inline void TrySetNeighborDistance(std::queue<Coords> &q, unsigned row, unsigned col, unsigned digit, unsigned distance)
{
    if (maze[row][col] != '#' && distance < dists[row][col][digit])
    {
        dists[row][col][digit] = distance;
        q.push(Coords{ row , col });
    }
}

void ComputeDistancesFromTargets()
{
    for (auto digit = 0U; digit < DIGITS; digit++)
    {
        std::queue<Coords> q;
        dists[targets[digit].row][targets[digit].col][digit] = 0;
        q.push(targets[digit]);

        while (!q.empty())
        {
            Coords coords = q.front();
            q.pop();

            auto row = coords.row, col = coords.col;
            auto distance = dists[row][col][digit] + 1;

            TrySetNeighborDistance(q, row - 1, col, digit, distance);
            TrySetNeighborDistance(q, row + 1, col, digit, distance);
            TrySetNeighborDistance(q, row, col - 1, digit, distance);
            TrySetNeighborDistance(q, row, col + 1, digit, distance);
        }
    }
}

inline unsigned GetCost(unsigned fromDigit, unsigned toDigit)
{
    return dists[targets[fromDigit].row][targets[fromDigit].col][toDigit];
}

inline unsigned GetCost(const std::string &path)
{
    unsigned cost = 0;
    for (auto i = 0U; i < path.length() - 1; i++)
        cost += GetCost(path[i] - '0', path[i + 1] - '0');

    return cost;
}

void Solve(const std::string &filename, unsigned &part1, unsigned &part2)
{
    ReadMaze(filename);
    InitializeMetrics();
    ComputeDistancesFromTargets();

    part1 = part2 = UINT_MAX;
    auto perm = "01234567"s;

    do
    {
        auto cost1 = GetCost(perm);
        auto cost2 = cost1 + GetCost(perm[perm.length() - 1] - '0', 0);

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
