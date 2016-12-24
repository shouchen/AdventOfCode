#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cassert>

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

bool DoGeneration(unsigned generation)
{
    bool didSomething = false;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            for (int k = 0; k < DIGITS; k++)
                if (dists[i][j][k] == generation - 1)
                {
                    if (i > 0 && maze[i - 1][j] != '#' && dists[i - 1][j][k] > generation)
                        dists[i - 1][j][k] = generation, didSomething = true;;

                    if (i < ROWS - 1 && maze[i + 1][j] != '#' &&dists[i + 1][j][k] > generation)
                        dists[i + 1][j][k] = generation, didSomething = true;;

                    if (j > 0 && maze[i][j - 1] != '#' &&dists[i][j - 1][k] > generation)
                        dists[i][j - 1][k] = generation, didSomething = true;;

                    if (j < COLS - 1 && maze[i][j + 1] != '#' &&dists[i][j + 1][k] > generation)
                        dists[i][j + 1][k] = generation, didSomething = true;;
                }

    return didSomething;
}

unsigned DeterminePathCost(const std::string path)
{
    unsigned cost = 0;
    for (auto i = 0U; i < path.length() - 1; i++)
    {
        unsigned target1 = path[i] - '0', target2 = path[i + 1] - '0';
        unsigned leg = dists[targets[target1].row][targets[target1].col][target2];
        cost += leg;
    }

    return cost;
}

unsigned DoPart1()
{
    // Go through all permutations of 1..7 starting from zero
    unsigned cheapest = UINT_MAX;
    std::string perm = "01234567";

    do
    {
        auto cost = DeterminePathCost(perm);
        if (cost < cheapest)
            cheapest = cost;
    } while (std::next_permutation(perm.begin() + 1, perm.end()));

    return cheapest;
}

unsigned DoPart2()
{
    // Go through all permutations of 1..7 starting from zero
    unsigned cheapest = UINT_MAX;
    std::string perm = "012345670";
    do
    {
        auto cost = DeterminePathCost(perm);
        if (cost < cheapest)
            cheapest = cost;
    } while (std::next_permutation(perm.begin() + 1, perm.end() - 1));

    return cheapest;
}

int _tmain(int argc, _TCHAR *argv[])
{
    double startTime = clock();

    std::ifstream f("input.txt");

    // Read maze
    std::string line;
    while (std::getline(f, line))
    {
        maze.push_back(line);
        // TODO: Figure out if 7 is correct
    }

    // Initialize dist metrics and digit table
    for (auto i = 0U; i < ROWS; i++)
    {
        for (auto j = 0U; j < COLS; j++)
        {
            for (auto k = 0U; k < DIGITS; k ++)
                dists[i][j][k] = UINT_MAX;

            if (isdigit(maze[i][j]))
            {
                unsigned target = maze[i][j] - '0';
                dists[i][j][target] = 0;
                targets[target] = { i, j };
            }
        }
    }

    // Determine distances from any square to any number target
    unsigned generation = 1;
    while (DoGeneration(generation))
        generation++;

    auto part1 = DoPart1();
    std::cout << "Part One: " << part1 << std::endl;

    // Similar thing for part 2
    unsigned part2 = DoPart2();
    std::cout << "Part Two: " << part2 << std::endl;

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;

    assert(part1 == 502);
    assert(part2 == 724);
    return 0;
}
