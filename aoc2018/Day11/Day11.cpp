#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include <stack>
#include <string>
#include <queue>
#include <map>
#include <numeric>
#include <algorithm>
#include <cassert>

std::vector<std::pair<int, int>> locations;
std::vector<std::pair<int, int>> velocities;

int grid[301][301];

int get_power(unsigned x, unsigned y, int gsn)
{
    int rack_id = x + 10;
    int power = rack_id * y;
    power += gsn;
    power *= rack_id;
    power = power / 100 % 10;
    power -= 5;
    return power;
}

int get_power_of_3x3(unsigned x, unsigned y, int gsn)
{
    int power = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            power += get_power(x + j, y + i, gsn);
        }
    return power;
}

std::pair<unsigned, unsigned> part1;
void do_part1()
{
    int highest_total_power = std::numeric_limits<int>::min();
    unsigned best_x = 0, best_y = 0;

    for (int y = 1; y < 298; y++)
        for (int x = 1; x < 298; x++)
        {
            auto power = get_power_of_3x3(x, y, 9306);
            if (power > highest_total_power)
            {
                highest_total_power = power;
                best_x = x;
                best_y = y;
            }
        }

    std::cout << best_x << "," << best_y << std::endl;
}



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
        auto y = atoi(p = strchr(p, ',') + 1);
        auto vx = atoi(p = strchr(p, '<') + 1);
        auto vy = atoi(strchr(p, ',') + 1);

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

    unsigned long long width = right - left, height = bottom - top;
    return width * height;
}

int main()
{
    do_part1();

    //read_input("input.txt");

    //auto time = 0U;
    //auto area = std::numeric_limits<unsigned long long>::max();

    //for (;;)
    //{
    //    if (right - left < 100 && bottom - top < 100)
    //        output_grid();

    //    auto new_area = update_locations_and_bounds_and_compute_area();
    //    if (new_area > area)
    //        break;

    //    area = new_area;
    //    time++;
    //}

    //std::cout << "After " << time << " seconds" << std::endl;

    //auto part1 = do_part1();
    //auto part2 = do_part2();

    //std::cout << "Part One: " << part1 << std::endl;
    //std::cout << "Part Two: " << part2 << std::endl;

    //assert(part1 == 561);
    //assert(part2 == 563);
    return 0;  //235,38
}
