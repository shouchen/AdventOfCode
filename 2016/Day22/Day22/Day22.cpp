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

struct Coords
{
    int x, y;
};

#define X_SIZE 39
#define Y_SIZE 25
Node grid[Y_SIZE][X_SIZE];
auto goalX = X_SIZE - 1U, goalY = 0U;

void Dump()
{
    for (auto i = 0U; i < Y_SIZE; i++)
    {
        for (auto j = 0U; j < X_SIZE; j++)
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
}

unsigned DoPart1()
{
    unsigned count = 0;
    for (auto y = 0; y < Y_SIZE; y++)
        for (auto x = 0; x < X_SIZE; x++)
        {
            if (grid[y][x].used == 0) continue;

            for (auto yy = 0; yy < Y_SIZE; yy++)
                for (auto xx = 0; xx < X_SIZE; xx++)
                {
                    if (y == yy && x == xx)
                        continue;
                    if (grid[y][x].used > grid[yy][xx].avail)
                        continue;
                    count++;
                }
        }

    return count;
}

unsigned numMoves;

std::string *ExtendPath(std::string path[Y_SIZE][X_SIZE], const Coords &from, const Coords &to, unsigned generation, char c)
{
    if ((to.y < 0) || (to.y >= Y_SIZE) || (to.x < 0) || (to.x >= X_SIZE))
        return nullptr;

    if (!(path[to.y][to.x].length() > generation + 1 || path[to.y][to.x].length() == 0) ||
        !(grid[to.y][to.x].avail + grid[to.y][to.x].used >= grid[from.y][from.x].used) ||
        !(to.y != goalY || to.x != goalX))
    {
        return nullptr;
    }

    path[to.y][to.x] = path[from.y][from.x] + c;
    
    if (grid[to.y][to.x].used != 0)
        return nullptr;

    return &path[to.y][to.x];
}

/*
inline void TryUpdateDistance(std::queue<Coords> &q, const Coords &coords, unsigned digit, unsigned newDistance)
{
    auto &oldDistance = distFromDigit[coords.row][coords.col][digit];

    if (maze[coords.row][coords.col] != '#' && newDistance < oldDistance)
    {
        oldDistance = newDistance;
        q.push(coords);
    }
}
*/

// Finds shortest path through the grid (xFrom, yFrom) to an empty disk but avoiding (goalX, goalY).
// Returns the move string so it can be replayed.
std::string FindPath(unsigned xFrom, unsigned yFrom, unsigned spaceNeeded)
{
    std::string path[Y_SIZE][X_SIZE];
    unsigned generation = 1;
    std::string retval;

    path[yFrom][xFrom] = " ";

    for (;;)
    {
        for (auto y = 0; y < Y_SIZE; y++)
        {
            for (auto x = 0; x < X_SIZE; x++)
            {
                if (x == goalX && y == goalY) continue;

                if (path[y][x].length() == generation)
                {
                    std::string *s;

                    Coords from{ x, y };

                    if ((s = ExtendPath(path, from, Coords{ x, y - 1 }, generation, 'U')) ||
                        (s = ExtendPath(path, from, Coords{ x, y + 1 }, generation, 'D')) ||
                        (s = ExtendPath(path, from, Coords{ x - 1, y }, generation, 'L')) ||
                        (s = ExtendPath(path, from, Coords{ x + 1, y }, generation, 'R')))
                    {
                        return *s;
                    }
                }
            }
        }

        generation++;
    }

    return retval;
}

/*
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
*/

void MakeMove(unsigned fromX, unsigned fromY, unsigned toX, unsigned toY)
{
    auto numToMove = grid[fromY][fromX].used;

    grid[fromY][fromX].used = 0;
    grid[fromY][fromX].avail += numToMove;
    grid[toY][toX].used += numToMove;
    grid[toY][toX].avail -= numToMove;

    numMoves++;
}

void ApplyMoves(std::string path, unsigned numToMove)
{
    // Do the last step and cut it out of the string
    while (path.length() > 1)
    {
        unsigned tempX = goalX - 1, tempY = goalY;
        auto c = path.begin() + 1;
        for (; c + 1 != path.end(); c++)
        {
            if (*c == 'U') tempY--;
            if (*c == 'D') tempY++;
            if (*c == 'L') tempX--;
            if (*c == 'R') tempX++;
        }

        // Do move from (tempX, tempY) -> (destX, destY)
        unsigned destX = tempX, destY = tempY;
        if (*c == 'L') destX = tempX - 1; else if (*c == 'R') destX = tempX + 1;
        if (*c == 'U') destY = tempY - 1; else if (*c == 'D') destY = tempY + 1;

        MakeMove(tempX, tempY, destX, destY);
        path.pop_back();
    }
}

unsigned DoPart2()
{
    while (goalX != 0 || goalY != 0)
    {
        auto numToMove = grid[goalY][goalX].used;
        if (grid[goalY][goalX - 1].avail < numToMove)
        {
            auto path = FindPath(goalX - 1, goalY, grid[goalY][goalX].used);
            assert(path.length() >= 1);

            ApplyMoves(path, numToMove);
        }

        MakeMove(goalX, goalY, goalX - 1, goalY);
        goalX--;
    }

    return numMoves;
}

int main()
{
    double startTime = clock();

    ReadInput("input.txt");

    auto part1 = DoPart1();
    auto part2 = DoPart2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;

    assert(part1 == 941);
    assert(part2 == 249);
    return 0;
}
