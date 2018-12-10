#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <numeric>
#include <algorithm>
#include <cassert>

std::vector<std::pair<int, int>> locations;
std::vector<std::pair<int, int>> velocities;

auto left = std::numeric_limits<int>::max(), right = std::numeric_limits<int>::min();
auto top = std::numeric_limits<int>::max(), bottom = std::numeric_limits<int>::min();

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        auto p = strchr(line.c_str(), '<') + 1;
        auto x = atoi(p);

        p = strchr(p, ',') + 1;
        auto y = atoi(p);

        p = strchr(p, '<') + 1;
        auto vx = atoi(p);

        p = strchr(p, ',') + 1;
        auto vy = atoi(p);

        locations.push_back(std::make_pair(x, y));
        velocities.push_back(std::make_pair(vx, vy));

        left = std::min(left, x);
        right = std::max(right, x);
        top = std::min(top, y);
        bottom = std::max(bottom, y);
    }
}

void output_grid()
{
    for (auto y = top; y <= bottom; y++)
    {
        for (auto x = left; x <= right; x++)
            std::cout << (std::find(locations.begin(), locations.end(), std::make_pair(x, y)) == locations.end() ? '.' : '#');

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

unsigned long long update_locations_and_bounds_and_compute_area()
{
    left = top = std::numeric_limits<int>::max(), right = bottom = std::numeric_limits<int>::min();

    for (auto i = 0; i < locations.size(); i++)
    {
        auto x = locations[i].first += velocities[i].first;
        auto y = locations[i].second += velocities[i].second;

        left = std::min(left, x);
        right = std::max(right, x);
        top = std::min(top, y);
        bottom = std::max(bottom, y);
    }

    return static_cast<unsigned long long>(right - left) * static_cast<unsigned long long>(bottom - top);
}

int main()
{
    read_input("input.txt");

    auto time = 0U;
    auto area = std::numeric_limits<unsigned long long>::max();

    for (;;)
    {
        if (right - left < 100 && bottom - top < 100)
            output_grid();

        auto new_area = update_locations_and_bounds_and_compute_area();
        if (new_area > area)
            break;

        area = new_area;
        time++;
    }

    std::cout << "After " << time << " seconds" << std::endl;

    // Visually, you should see "GPEPPPEJ" for this input data.
    assert(time == 10101);
    return 0;
}
