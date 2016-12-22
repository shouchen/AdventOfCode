#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <ctime>
#include <cassert>

struct Node
{
    unsigned used, avail;
};

#define X_SIZE 39
#define Y_SIZE 25
//#define X_SIZE 3
//#define Y_SIZE 3
Node grid[Y_SIZE][X_SIZE];
auto goalX = X_SIZE - 1U, goalY = 0U;

void Dump()
{
    for (int i = 0; i < Y_SIZE; i++)
    {
        for (int j = 0; j < X_SIZE; j++)
        {
            std::cout << std::setw(3) << grid[i][j].used << "/" << std::setw(3) << (grid[i][j].used + grid[i][j].avail);
            std::cout << ((i == goalY && j == goalX) ? "*" : " ");
            std::cout << "|";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void ReadInput(const std::string &filename)
{
    std::ifstream f(filename);
    std::string path, line;
    unsigned size, used, avail, usedpct;
    char c;

    getline(f, line);
    getline(f, line);

    std::vector<Node> nodes;
    while (f >> path >> size >> c >> used >> c >> avail >> c >> usedpct >> c)
    {
        // Part1
        nodes.push_back(Node{ used, avail });

        auto temp = path.find("-y") + 2;
        auto temp1 = &path[temp];

        auto x = atoi(&path[path.find("-x") + 2]);
        auto y = atoi(&path[path.find("-y") + 2]);
        grid[y][x].used = used;
        grid[y][x].avail = avail;

    }

#if 0
    unsigned count = 0;
    for (auto a = nodes.begin(); a != nodes.end(); a++)
    {
        if (a->used == 0) continue;

        for (auto b = nodes.begin(); b != nodes.end(); b++)
        {
            if (a == b) 
                continue;
            if (a->used > b->avail) 
                continue;
            count++;

        }
    }

    std::cout << count << std::endl; // 941 for part1
#endif
}

unsigned numMoves;
// Finds shortest path through the grid (xFrom, yFrom) to an empty disk but avoiding (goalX, goalY).
// Returns the move string to it can be replayed.
std::string FindPath(unsigned xFrom, unsigned yFrom, unsigned spaceNeeded)
{
    std::string path[Y_SIZE][X_SIZE];
    unsigned generation = 1;

    path[yFrom][xFrom] = " ";

    for (;;)
    {
        for (int y = 0; y < Y_SIZE; y++)
            for (int x = 0; x < X_SIZE; x++)
            {
                if (x == goalX && y == goalY) continue;

                if (path[y][x].length() == generation)
                {
                    if (y > 0 &&
                        (path[y - 1][x].length() > generation + 1 || path[y - 1][x].length() == 0) &&
                        (grid[y - 1][x].avail + grid[y - 1][x].used >= grid[y][x].used) &&
                        (y - 1 != goalY || x != goalX))
                    {
                        path[y - 1][x] = path[y][x] + 'U';
                        if (grid[y - 1][x].used == 0)
                            return path[y - 1][x];
                    }
                    if (y < Y_SIZE - 1 &&
                        (path[y + 1][x].length() > generation + 1 || path[y + 1][x].length() == 0) &&
                        (grid[y + 1][x].avail + grid[y + 1][x].used >= grid[y][x].used) &&
                        (y + 1 != goalY || x != goalX))
                    {
                        path[y + 1][x] = path[y][x] + 'D';
                        if (grid[y + 1][x].used == 0)
                            return path[y + 1][x];
                    }
                    if (x > 0 &&
                        (path[y][x - 1].length() > generation + 1 || path[y][x - 1].length() == 0) &&
                        (grid[y][x - 1].avail + grid[y][x - 1].used >= grid[y][x].used) &&
                        (y != goalY || x - 1 != goalX))
                    {
                        path[y][x - 1] = path[y][x] + 'L';
                        if (grid[y][x - 1].used == 0)
                            return path[y][x - 1];
                    }
                    if (x < X_SIZE - 1 &&
                        (path[y][x + 1].length() > generation + 1 || path[y][x + 1].length() == 0) &&
                        (grid[y][x + 1].avail + grid[y][x + 1].used >= grid[y][x].used) &&
                        (y != goalY || x + 1 != goalX))
                    {
                        path[y][x + 1] = path[y][x] + 'R';
                        if (grid[y][x + 1].used == 0)
                            return path[y][x + 1];
                    }
                }
            }

        generation++;
    }

    return "";
}

void MoveGoalLeft()
{
    auto numToMove = grid[goalY][goalX].used;
    if (grid[goalY][goalX - 1].avail < numToMove)
    {
        auto path = FindPath(goalX - 1, goalY, grid[goalY][goalX].used);
        assert(path.length() >= 1);
        std::cout << "Doing" << path << " (" << path.length() - 1 << " steps)" << std::endl;

        // Do the last step and cut it out of the string
        while (path.length() > 1)
        {
            unsigned tempX = goalX - 1, tempY = goalY;
            auto c = path.begin() + 1;
            for (; c + 1 != path.end(); c++)
            {
                if (*c == 'U')
                    tempY--;
                if (*c == 'D')
                    tempY++;
                if (*c == 'L')
                    tempX--;
                if (*c == 'R')
                    tempX++;
            }

            // Do move from (tempX, tempY) -> (destX, destY)
            unsigned destX = tempX, destY = tempY;
            if (*c == 'L') destX = tempX - 1; else if (*c == 'R') destX = tempX + 1;
            if (*c == 'U') destY = tempY - 1; else if (*c == 'D') destY = tempY + 1;

            numToMove = grid[tempY][tempX].used;
            assert(grid[destY][destX].avail >= numToMove);

            grid[tempY][tempX].used = 0;
            grid[tempY][tempX].avail += numToMove;
            grid[destY][destX].used += numToMove;
            grid[destY][destX].avail -= numToMove;
            numMoves++;

            path.pop_back();

            //Dump();
        }
    }

    std::cout << "MoveGoalLeft(" << goalX << ", " << goalY << ") " << numMoves << std::endl;

    grid[goalY][goalX].used = 0;
    grid[goalY][goalX].avail += numToMove;
    goalX--;
    grid[goalY][goalX].used += numToMove;
    grid[goalY][goalX].avail -= numToMove;
    numMoves++;

    //Dump();
}

void Part2()
{
    Dump();

    while (goalX != 0 || goalY != 0)
        MoveGoalLeft();

    std::cout << "answer = " << numMoves << std::endl;
}

int main()
{
    double startTime = clock();

    ReadInput("input.txt");
    Part2();

    //auto part1 = 0U, part2 = 0U;
    //Solve(part1, part2);

    //std::cout << "Part One: " << part1 << std::endl;
    //std::cout << "Part Two: " << part2 << std::endl;

    //std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;

    //assert(part1 == 32259706);
    //assert(part2 == 113);
    return 0;
}

// 249 was right for part 2!
