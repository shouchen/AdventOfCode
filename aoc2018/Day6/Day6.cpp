#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

void read_input(std::vector<std::pair<int, int>> &initial, unsigned &left, unsigned &top, unsigned &right, unsigned &bottom)
{
    left = std::numeric_limits<unsigned>::max();
    right = std::numeric_limits<unsigned>::min();
    top = std::numeric_limits<unsigned>::max();
    bottom = std::numeric_limits<unsigned>::min();
    initial.clear();

    std::ifstream file("input.txt");

    auto x = 0U, y = 0U;
    auto comma = ',';

    while (file >> x >> comma >> y)
    {
        auto loc = std::make_pair(y, x);
        initial.push_back(loc);

        left = std::min(x, left);
        right = std::max(x, right);
        top = std::min(y, top);
        bottom = std::max(y, bottom);
    }
}

auto get_manhattan_dist(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

auto get_closest_area(unsigned x, unsigned y, int grid[500][500], const std::vector<std::pair<int, int>> &initial)
{
    auto closest = std::numeric_limits<unsigned>::max();
    auto closest_area = -1;

    for (auto &item : initial)
    {
        auto dist = get_manhattan_dist(item.second, item.first, x, y);
        if (dist < closest)
        {
            closest = dist;
            closest_area = grid[item.first][item.second];
        }
        else if (dist == closest)
        {
            closest_area = -1;
        }
    }

    return closest_area;
}

auto do_part1()
{
    // TODO: Make this a map of x,y -> area # and eliminate the initial vector?
    int initial_grid[500][500] = { 0 };
    auto left = 0U, top = 0U, right = 0U, bottom = 0U;
    std::vector<std::pair<int, int>> initial;

    read_input(initial, left, top, right, bottom);

    // put initial coords into grid
    auto area = 1U;
    for (auto item : initial)
        initial_grid[item.first][item.second] = area++;

    // count area sizes and also mark those that intersect with outer bounds (infinite expansion)
    std::map<char, unsigned> counts;
    std::set<char> infinite;
    for (auto y = top; y <= bottom; y++)
        for (auto x = left; x <= right; x++)
        {
            auto a = initial_grid[y][x];
            if (!a)
                a = get_closest_area(x, y, initial_grid, initial);

            if (a != -1)
            {
                counts[a]++;
                if (y == top || y == bottom || x == left || x == right)
                    infinite.insert(a);
            }
        }

    auto max = 0U;
    for (auto &item : counts)
        if (item.second > max && infinite.find(item.first) == infinite.end())
            max = item.second;

    return max;
}

auto do_part2()
{
    auto left = 0U, top = 0U, right = 0U, bottom = 0U;
    std::vector<std::pair<int, int>> initial;
    read_input(initial, left, top, right, bottom);

    auto count = 0U;
    for (auto y = top; y <= bottom; y++)
        for (auto x = left; x <= right; x++)
        {
            auto dist_from_all_areas = 0;
            for (auto &item : initial)
                dist_from_all_areas += get_manhattan_dist(item.second, item.first, x, y);
            if (dist_from_all_areas < 10000)
                count++;
        }

    return count;
}

int main()
{
    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 3293);
    assert(part2 == 45176);
    return 0;
}
