#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <set>
#include <math.h>

struct xypair
{
    int x, y;

    xypair(int x = 0, int y = 0) { this->x = x; this->y = y; }
    int blocks_from_origin() const { return abs(x) + abs(y); }
    bool operator<(const xypair &rhs) const
    {
        if (x < rhs.x) return true;
        if (x > rhs.x) return false;
        return y < rhs.y;
    }
    xypair operator+=(const xypair &rhs) { x += rhs.x; y += rhs.y; return *this; }
};

xypair location(0, 0), direction(0, -1);
std::set<xypair> visited;
std::set<xypair>::iterator first_revisited = visited.end();

void process_file(const std::string &filename)
{
    visited.insert(location);

    std::ifstream f(filename);

    char turn, comma;
    int distance;

    while (f >> turn >> distance)
    {
        std::swap(direction.x, direction.y);
        if (turn == 'R') direction.x = -direction.x; else direction.y = -direction.y;

        while (distance--)
        {
            location += direction;

            std::set<xypair>::iterator found = visited.find(location);

            if (found == visited.end())
                visited.insert(location);
            else if (first_revisited == visited.end())
                first_revisited = found;
        }

        f >> comma;
    }
}

int main()
{
    process_file("input.txt");

    auto part_one = location.blocks_from_origin();
    auto part_two = (first_revisited != visited.end()) ? first_revisited->blocks_from_origin() : -1;

    std::cout << "Part One: " << part_one << std::endl;
    std::cout << "Part Two: " << part_two << std::endl;

    assert(part_one == 299);
    assert(part_two == 181);
    return 0;
}
