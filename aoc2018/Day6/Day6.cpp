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

void read_input(std::vector<std::pair<int, int>> &initial, unsigned &left, unsigned &top, unsigned &right, unsigned &bottom)
{
    left = std::numeric_limits<unsigned>::max();
    right = std::numeric_limits<unsigned>::min();
    top = std::numeric_limits<unsigned>::max();
    bottom = std::numeric_limits<unsigned>::min();
    initial.clear();

    std::ifstream file("input.txt");

    auto x = 0U, y = 0U;
    char comma = ',';

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
    auto left = 0U, top = 0U, right = 0U, bottom = 0U;
    std::vector<std::pair<int, int>> initial;

    read_input(initial, left, top, right, bottom);

    auto area = 1U;
    for (auto item : initial)
        grid[item.first][item.second] = area++;

    for (auto y = top; y <= bottom; y++)
        for (auto x = left; x <= right; x++)
            if (grid[y][x] < 1)
                grid[y][x] = get_closest_area(x, y, grid, initial);

    // count area sizes and find those that intersect with outer bounds (infinite expansion)
    std::map<char, unsigned> counts;
    std::set<char> infinite;
    for (auto y = top; y <= bottom; y++)
        for (auto x = left; x <= right; x++)
        {
            auto c = grid[y][x];
            if (c != '.')
            {
                counts[c]++;
                if (y == top || y == bottom || x == left || x == right)
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
    auto left = 0U, top = 0U, right = 0U, bottom = 0U;
    std::vector<std::pair<int, int>> initial;
    read_input(initial, left, top, right, bottom);

    auto count = 0U;
    for (auto y = top; y <= bottom; y++)
        for (auto x = left; x <= right; x++)
        {
            auto total_dist = 0;
            for (auto &item : initial)
                total_dist += get_manhattan_dist(item.second, item.first, x, y);
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
