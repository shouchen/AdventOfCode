#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

unsigned get_manhattan_dist(unsigned x1, unsigned y1, unsigned x2, unsigned y2)
{
    auto x = (x1 > x2) ? (x1 - x2) : (x2 - x1);
    auto y = (y1 > y2) ? (y1 - y2) : (y2 - y1);
    return x + y;
}

void read_input(std::vector<std::pair<int, int>> &initial, unsigned &min_x, unsigned &min_y, unsigned &max_x, unsigned &max_y)
{
    min_x = std::numeric_limits<unsigned>::max();
    max_x = std::numeric_limits<unsigned>::min();
    min_y = std::numeric_limits<unsigned>::max();
    max_y = std::numeric_limits<unsigned>::min();
    initial.clear();

    std::ifstream file("input.txt");

    auto x = 0U, y = 0U;
    char comma = ',';

    while (file >> x >> comma >> y)
    {
        auto loc = std::make_pair(y, x);
        initial.push_back(loc);

        min_x = std::min(x, min_x);
        max_x = std::max(x, max_x);
        min_y = std::min(y, min_y);
        max_y = std::max(y, max_y);
    }
}

int get_closest_area(unsigned x, unsigned y, int grid[500][500], const std::vector<std::pair<int, int>> &initial)
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

unsigned do_part1()
{
    int grid[500][500] = { 0 };
    auto min_x = 0U, min_y = 0U, max_x = 0U, max_y = 0U;
    std::vector<std::pair<int, int>> initial;

    read_input(initial, min_x, min_y, max_x, max_y);

    auto area = 1U;
    for (auto item : initial)
        grid[item.first][item.second] = area++;

    for (auto y = min_y; y <= max_y; y++)
        for (auto x = min_x; x <= max_x; x++)
            if (grid[y][x] < 1)
                grid[y][x] = get_closest_area(x, y, grid, initial);

    // count area sizes and find those that intersect with outer bounds (infinite expansion)
    std::map<char, unsigned> counts;
    std::set<char> infinite;
    for (auto y = min_y; y <= max_y; y++)
        for (auto x = min_x; x <= max_x; x++)
        {
            auto c = grid[y][x];
            if (c != '.')
            {
                counts[c]++;
                if (y == min_y || y == max_y || x == min_x || x == max_x)
                    infinite.insert(c);
            }
        }

    auto max = 0U;
    for (auto &item : counts)
        if (item.second > max && infinite.find(item.first) == infinite.end())
            max = item.second;

    return max;
}

unsigned do_part2()
{
    auto min_x = 0U, min_y = 0U, max_x = 0U, max_y = 0U;
    std::vector<std::pair<int, int>> initial;
    read_input(initial, min_x, min_y, max_x, max_y);

    auto count = 0U;
    for (auto row = min_y; row <= max_y; row++)
        for (auto col = min_x; col <= max_x; col++)
        {
            auto total_dist = 0;
            for (auto &item : initial)
                total_dist += get_manhattan_dist(item.second, item.first, col, row);
            if (total_dist < 10000)
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
