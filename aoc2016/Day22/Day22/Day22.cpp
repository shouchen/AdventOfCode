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
    unsigned x, y;
    bool operator==(const Coords &rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Coords &rhs) const { return !operator==(rhs); }
};

std::vector<std::vector<Node>> grid;

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
}

void MakeMove(const Coords &from, const Coords &to)
{
    auto numToMove = grid[from.y][from.x].used;

    grid[from.y][from.x].used = 0;
    grid[from.y][from.x].avail += numToMove;
    grid[to.y][to.x].used += numToMove;
    grid[to.y][to.x].avail -= numToMove;
}

// If this completes the path, the non-zero length of the path is returned.
unsigned ExtendPath(
    std::queue<Coords> &q,                   // BFS queue for shortest path
    std::vector<std::vector<Coords>> &prev,  // table of previous for shortest path
    const Coords &from,                      // previous grid location
    const Coords &to,                        // potentially next grid location
    const Coords &start,                     // the start for this entire path
    const Coords &goal)                      // the goal (the "avoid" space on the grid
{
    // Make sure not off the board entirely, and make sure path doesn't go through goal.
    if (to.x >= grid[0].size() || to.y >= grid.size() || to == goal)
        return 0;

    // To avoid loops, don't extend to an already-visited spot.
    if (prev[to.y][to.x].x != UINT_MAX || prev[to.y][to.x].y != UINT_MAX)
        return 0;

    // Make sure this intermediate square would have enough space for the entire source to pass through here.
    if (!(grid[to.y][to.x].avail + grid[to.y][to.x].used >= grid[from.y][from.x].used))
        return 0;

    // Extend the path to this one by marking its prev as however we got here.
    prev[to.y][to.x] = from;
    q.push(to);
    
    // If this is not an empty one, have to keep going.
    if (grid[to.y][to.x].used != 0)
        return 0;

    // This is the empty one; we're done.
    prev[to.y][to.x] = from;

    // Found cheapest way to open a path; do the requisite moves.
    auto numMoves = 0U;
    for (auto curr = to; curr != start; curr = prev[curr.y][curr.x])
    {
        MakeMove(prev[curr.y][curr.x], curr);
        numMoves++;
    }

    return numMoves;
}

// Frees up grid at indicated spot by shifting data around (lowest cost). Returns numMoves.
unsigned OpenPath(const Coords &at, const Coords &goal)
{
    auto numMoves = 0U;
    std::vector<std::vector<Coords>> prev;

    for (auto y = 0U; y < grid.size(); y++)
    {
        prev.push_back(std::vector<Coords>());
        for (auto x = 0U; x < grid[0].size(); x++)
            prev[prev.size() - 1].push_back(Coords{ UINT_MAX, UINT_MAX });
    }

    std::queue<Coords> q;
    q.push(at);

    while (!q.empty())
    {
        Coords from = q.front();
        q.pop();

        if ((ExtendPath(q, prev, from, Coords{ from.x, from.y - 1 }, at, goal)) ||
            (ExtendPath(q, prev, from, Coords{ from.x, from.y + 1 }, at, goal)) ||
            (ExtendPath(q, prev, from, Coords{ from.x - 1, from.y }, at, goal)) ||
            (ExtendPath(q, prev, from, Coords{ from.x + 1, from.y }, at, goal)))
        {
            return numMoves;
        }
    }

    return 0;
}

unsigned DoPart1()
{
    unsigned count = 0;

    for (auto y = 0U; y < grid.size(); y++)
        for (auto x = 0U; x < grid[y].size(); x++)
            if (grid[y][x].used)
                for (auto yy = 0U; yy < grid.size(); yy++)
                    for (auto xx = 0U; xx < grid[yy].size(); xx++)
                        if ((y != yy || x != xx) &&
                            (grid[y][x].used <= grid[yy][xx].avail))
                        {
                            count++;
                        }

    return count;
}

unsigned DoPart2()
{
    auto numMoves = 0U;

    Coords goal = { grid[0].size() - 1, 0 };
    while (goal.x != 0)
    {
        auto newGoal = Coords{ goal.x - 1, goal.y };

        // If the spot left of the goal is not open, free it up first
        if (grid[newGoal.y][newGoal.x].avail < grid[goal.y][goal.x].used)
            numMoves += OpenPath(newGoal, goal);

        // Move goal left into the open spot
        MakeMove(goal, newGoal);
        numMoves++;
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
