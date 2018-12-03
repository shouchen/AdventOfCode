#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cassert>

struct Coords
{
    int x, y, z, t;
};

std::vector<Coords> stars;
std::set<const Coords *> visited;

auto get_dist(const Coords *p1, const Coords *p2)
{
    return abs(p1->x - p2->x) + abs(p1->y - p2->y) + abs(p1->z - p2->z) + abs(p1->t - p2->t);
}

void visit(const Coords *star)
{
    visited.insert(star);
    for (const auto &other : stars)
        if (get_dist(star, &other) <= 3 && visited.find(&other) == visited.end())
            visit(&other);
}

int main()
{
    std::ifstream file("input.txt");
    auto x = 0, y = 0, z = 0, t = 0;
    auto comma = ',';

    while (file >> x >> comma >> y >> comma >> z >> comma >> t)
        stars.push_back(Coords{ x, y, z, t });

    auto num_constellations = 0;
    for (const auto &star : stars)
        if (visited.find(&star) == visited.end())
        {
            num_constellations++;
            visit(&star);
        }

    std::cout << "Part 1: " << num_constellations << std::endl;
    assert(num_constellations == 375);
    return 0;
}
