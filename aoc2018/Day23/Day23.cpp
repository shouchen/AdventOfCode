#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <queue>
#include <string>
#include <numeric>
#include <algorithm>
#include <cassert>

struct Nanobot
{
    int x, y, z;
    int radius;
};

std::vector<Nanobot> nanobots;
int strongest = 0;

int min_x = std::numeric_limits<int>::max();
int max_x = std::numeric_limits<int>::min();
int min_y = std::numeric_limits<int>::max();
int max_y = std::numeric_limits<int>::min();
int min_z = std::numeric_limits<int>::max();
int max_z = std::numeric_limits<int>::min();

//pos=<0,0,0>, r=4

bool in_range_of_strongest(Nanobot &n)
{
    auto x = n.x - nanobots[strongest].x; if (x < 0) x = -x;
    auto y = n.y - nanobots[strongest].y; if (y < 0) y = -y;
    auto z = n.z - nanobots[strongest].z; if (z < 0) z = -z;

    return (x + y + z <= nanobots[strongest].radius);
}

bool in_range_of_each_other(Nanobot &n1, Nanobot &n2)
{
    auto x = n1.x - n2.x; if (x < 0) x = -x;
    auto y = n1.y - n2.y; if (y < 0) y = -y;
    auto z = n1.z - n2.z; if (z < 0) z = -z;

    return (x + y + z <= n1.radius + n2.radius);
}

unsigned get_num_nanobots_in_range(int x, int y, int z)
{
    unsigned count = 0;

    for (auto &n : nanobots)
    {
        auto xdist = n.x - x; if (x < 0) x = -x;
        auto ydist = n.y - y; if (y < 0) y = -y;
        auto zdist = n.z - z; if (z < 0) z = -z;

        if (xdist + ydist + zdist <= n.radius)
            count++;
    }

    return count;
}

std::map<std::pair<std::pair<int, int>, int>, int> num_in_range;  //((x, y), z) -> dist
unsigned max_in_range = 0;

void eval(int x, int y, int z)
{
    //std::cout << "Eval " << x << " " << y << " " << z << std::endl;

    auto index = std::make_pair(std::make_pair(x, y), z);
    unsigned num = 0;
    if (num_in_range.find(index) == num_in_range.end())
    {
        num = num_in_range[index] = get_num_nanobots_in_range(x, y, z);
    }
    else
    {
        num = num_in_range[index];
    }

    if (num > max_in_range)
    {
        max_in_range = num;
        std::cout << "New Max" << std::endl;
    }
}

int main()
{
    std::ifstream file("input.txt");
    int x, y, z, radius;
    char c, comma;

    while (file >> c >> c >> c >> c >> c >> x >> comma >> y >> comma >> z >> c >> comma >> c >> c >> radius)
    {
        nanobots.push_back(Nanobot{ x, y, z, radius });
        if (nanobots.back().radius > nanobots[strongest].radius)
            strongest = nanobots.size() - 1;
        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);
        min_z = std::min(min_z, z);
        max_z = std::max(max_z, z);
    }

    unsigned count = 0;
    for (int i = 0; i < nanobots.size(); i++)
    {
        //if (i == strongest)
        //    continue;
        if (in_range_of_strongest(nanobots[i]))
        {
            std::cout << "Part 1: " << i << std::endl;
            count++;
        }
    }

    // PART 2
    std::vector<unsigned> intersections;
    unsigned max_intersections = 0;
    for (int i = 0; i < nanobots.size(); i++)
    {
        unsigned count = 0;
        for (int j = 0; j < nanobots.size(); j++)
        {
            if (i == j) continue;

            if (in_range_of_each_other(nanobots[i], nanobots[j]))
            {
                count++;
            }
        }

        std::cout << "Bot " << i << " intersects " << count << " others" << std::endl;
        intersections.push_back(count);
        max_intersections = std::max(max_intersections, count);
    }

    std::cout << "Max intersections = " << max_intersections << std::endl;
    for (int i = 0; i < nanobots.size(); i++)
    {
        if (intersections[i] == max_intersections)
            std::cout << i << " ";
    }
    std::cout << std::endl;

    // Bot 740 has 991 intersections (the max)

    return 0;
}
// 780
// 110841112