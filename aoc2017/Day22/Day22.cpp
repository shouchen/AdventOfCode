#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

enum State { Clean, Weakened, Infected, Flagged };

struct XY
{
    int x, y;

    void TurnLeft() { std::swap(x, y); y = -y; }
    void TurnRight() { std::swap(x, y); x = -x; }
    void TurnAround() { x = -x; y = -y; }

    auto operator<(const XY &rhs) const
    {
        if (y < rhs.y) return true;
        if (y > rhs.y) return false;
        return x < rhs.x;
    }

    auto &operator+=(const XY &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
};

auto process_input(const std::string &filename, unsigned times, bool for_part1)
{
    std::map<XY, State> grid;
    std::ifstream f(filename);
    std::string line;
    auto x = 0, y = 0;

    while (getline(f, line))
    {
        x = 0;
        for (auto c : line)
        {
            if (c == '#') grid[{ x, y }] = Infected;
            x++;
        }
        y++;
    }

    auto curr = XY { x / 2, y / 2 }, dir = XY { 0, -1 };
    auto num_times_infected = 0U;

    for (auto i = 0U; i < times; i++)
    {
        auto found = grid.find(curr);
        auto state = found == grid.end() ? Clean : found->second;

        switch (state)
        {
        case Clean:
            dir.TurnLeft();
            if (for_part1)
                grid[curr] = Infected, num_times_infected++;
            else
                grid[curr] = Weakened;
            break;
        case Weakened:
            grid[curr] = Infected;
            num_times_infected++;
            break;
        case Infected:
            dir.TurnRight();
            if (for_part1)
                grid.erase(curr);
            else
                grid[curr] = Flagged;
            break;
        case Flagged:
            dir.TurnAround();
            grid.erase(curr);
            break;
        }

        curr += dir;
    }

    return num_times_infected;
}

int main()
{
    assert(process_input("input-test.txt", 7, true) == 5);
    assert(process_input("input-test.txt", 70, true) == 41);
    assert(process_input("input-test.txt", 10000, true) == 5587);

    assert(process_input("input-test.txt", 100, false) == 26);
    assert(process_input("input-test.txt", 10000000, false) == 2511944);

    auto part1 = process_input("input.txt", 10000, true);
    auto part2 = process_input("input.txt", 10000000, false);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 5246);
    assert(part2 == 2512059);
    return 0;
}
