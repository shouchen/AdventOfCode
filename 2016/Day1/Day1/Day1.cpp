#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <set>
#include <math.h>

struct Direction
{
    int x, y;

    Direction(int x = 0, int y = -1) { this->x = x; this->y = y; }
    void TurnLeft() { int temp = x; x = y; y = -temp; }
    void TurnRight() { int temp = x; x = -y; y = temp; }
};

struct Location
{
    int x, y;

    Location(int x = 0, int y = 0) { this->x = x; this->y = y; }
    int blocks_from_origin() const { return abs(x) + abs(y); }
    bool operator<(const Location &rhs) const
    {
        if (x < rhs.x) return true;
        if (x > rhs.x) return false;
        return y < rhs.y;
    }
    Location operator+=(const Direction &rhs) { x += rhs.x; y += rhs.y; return *this; }
};

Location location(0, 0);
Direction direction;

std::set<Location> visited;
const Location *first_revisited = nullptr;

void process_file(const std::string &filename)
{
    visited.insert(location);

    std::ifstream f(filename);

    char turn, comma;
    int distance;

    while (f >> turn >> distance)
    {
        (turn == 'L') ? direction.TurnLeft() : direction.TurnRight();

        while (distance--)
        {
            location += direction;
            auto found = visited.find(location);

            if (found == visited.end())
                visited.insert(location);
            else if (!first_revisited)
                first_revisited = &(*found);
        }

        f >> comma;
    }
}

int main()
{
    process_file("input.txt");

    auto part_one = location.blocks_from_origin();
    auto part_two = first_revisited ? first_revisited->blocks_from_origin() : -1;

    std::cout << "Part One: " << part_one << std::endl;
    std::cout << "Part Two: " << part_two << std::endl;

    assert(part_one == 299);
    assert(part_two == 181);
    return 0;
}
