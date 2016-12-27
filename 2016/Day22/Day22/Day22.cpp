#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
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
    bool operator==(const Coords &rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Coords &rhs) const { return !operator==(rhs); }
};

std::vector<std::vector<Node>> grid;
Coords goal;
unsigned numMoves;

void ReadInput(const std::string &filename)
{
    std::ifstream f(filename);
    std::string path, line;
    unsigned size, used, avail, usedpct;
    char c;

    getline(f, line);
    getline(f, line);

    while (f >> path >> size >> c >> used >> c >> avail >> c >> usedpct >> c)
    {
        unsigned x = atoi(&path[path.find("-x") + 2]);
        unsigned y = atoi(&path[path.find("-y") + 2]);

        if (grid.size() < y + 1) grid.push_back(std::vector<Node>());
        if (grid[y].size() < x + 1) grid[y].push_back(Node());

        grid[y][x].used = used;
        grid[y][x].avail = avail;
    }

    goal = { static_cast<int>(grid[0].size()) - 1, 0 };
}

void MakeMove(const Coords &from, const Coords &to)
{
    auto numToMove = grid[from.y][from.x].used;

    grid[from.y][from.x].used = 0;
    grid[from.y][from.x].avail += numToMove;
    grid[to.y][to.x].used += numToMove;
    grid[to.y][to.x].avail -= numToMove;

    numMoves++;
}

const Coords *ExtendPath(std::queue<Coords> &q, std::vector<std::vector<Coords>> &prev, const Coords &from, const Coords &to)
{
    // Make sure not off the board entirely.
    if ((to.y < 0) || (to.y >= grid.size()) || (to.x < 0) || (to.x >= grid[0].size()))
        return nullptr;

    // Don't extend to the goal (it blocks).
    if (to == goal)
        return nullptr;

    // To avoid loops, don't extend to an already-visited spot.
    if (prev[to.y][to.x].x != -1 || prev[to.y][to.x].y != -1)
        return nullptr;

    // Make sure this intermediate square would have enough space for the entire source to pass through here.
    if (!(grid[to.y][to.x].avail + grid[to.y][to.x].used >= grid[from.y][from.x].used))
        return nullptr;

    // Extend the path to this one by marking its prev as however we got here.
    prev[to.y][to.x] = from;
    q.push(to);
    
    // If this is not an empty one, have to keep going.
    if (grid[to.y][to.x].used != 0)
        return nullptr;

    // This is the empty one; we're done.
    prev[to.y][to.x] = from;
    return &to;
}

// Finds shortest path through the grid from coords to an empty disk but avoiding goal in path.
bool OpenPath(Coords coords)
{
    std::vector<std::vector<Coords>> prev;

    for (auto y = 0U; y < grid.size(); y++)
    {
        prev.push_back(std::vector<Coords>());
        for (auto x = 0U; x < grid[0].size(); x++)
            prev[prev.size() - 1].push_back(Coords{ -1,-1 });
    }

    std::queue<Coords> q;
    q.push(coords);

    while (!q.empty())
    {
        Coords from = q.front();
        q.pop();

        const Coords *pathEnd;

        if ((pathEnd = ExtendPath(q, prev, from, Coords{ from.x, from.y - 1 })) ||
            (pathEnd = ExtendPath(q, prev, from, Coords{ from.x, from.y + 1 })) ||
            (pathEnd = ExtendPath(q, prev, from, Coords{ from.x - 1, from.y })) ||
            (pathEnd = ExtendPath(q, prev, from, Coords{ from.x + 1, from.y })))
        {
            // Found shortest path to open disk; do the requisite moves and exit.
            for (auto curr = *pathEnd; curr != coords; curr = prev[curr.y][curr.x])
                MakeMove(prev[curr.y][curr.x], curr);

            return true;
        }
    }

    return false;
}

unsigned DoPart1()
{
    unsigned count = 0;

    for (auto y = 0; y < grid.size(); y++)
        for (auto x = 0; x < grid[y].size(); x++)
            if (grid[y][x].used)
                for (auto yy = 0; yy < grid.size(); yy++)
                    for (auto xx = 0; xx < grid[yy].size(); xx++)
                        if ((y != yy || x != xx) &&
                            (grid[y][x].used <= grid[yy][xx].avail))
                        {
                            count++;
                        }

    return count;
}

unsigned DoPart2()
{
    while (goal.x != 0)
    {
        auto newGoal = Coords{ goal.x - 1, goal.y };

        // If the spot left of the goal is not open, free it up first
        if (grid[newGoal.y][newGoal.x].avail < grid[goal.y][goal.x].used)
            OpenPath(newGoal);

        // Move goal left into the open spot
        MakeMove(goal, newGoal);
        goal = newGoal;
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
