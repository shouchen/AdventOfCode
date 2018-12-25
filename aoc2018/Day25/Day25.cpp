#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <cassert>

struct Coords
{
    int x, y, z, t;
};

std::vector<Coords> stars;
std::map<int, std::vector<int>> neighbors;
std::set<int> stars_merged;

auto get_manhattan(const Coords &p1, const Coords &p2)
{
    auto x = p1.x - p2.x; if (x < 0) x = -x;
    auto y = p1.y - p2.y; if (y < 0) y = -y;
    auto z = p1.z - p2.z; if (z < 0) z = -z;
    auto t = p1.t - p2.t; if (t < 0) t = -t;

    return x + y + z + t;
}

void walk_neighbors(int index)
{
    if (stars_merged.find(index) == stars_merged.end())
    {
        stars_merged.insert(index);
        for (auto i = 0; i < neighbors[index].size(); i++)
            walk_neighbors(neighbors[index][i]);
    }
}

int main()
{
    std::ifstream file("input.txt");
    auto x = 0, y = 0, z = 0, t = 0;
    auto comma = ',';

    while (file >> x >> comma >> y >> comma >> z >> comma >> t)
        stars.push_back(Coords{ x, y, z, t });

    for (auto i = 0; i < stars.size(); i++)
    {
        for (auto j = 0; j < i; j++)
        {
            if (get_manhattan(stars[i], stars[j]) <= 3)
            {
                neighbors[i].push_back(j);
                neighbors[j].push_back(i);
            }
        }
    }

    auto num_constellations = 0;
    for (auto i = 0; i < stars.size(); i++)
    {
        if (stars_merged.find(i) == stars_merged.end())
        {
            num_constellations++;
            walk_neighbors(i);
        }
    }

    std::cout << "Part 1: " << num_constellations << std::endl;

    assert(num_constellations == 375);
    return 0;
}
