#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

char grid1[500][500];

unsigned manhattan(unsigned x1, unsigned y1, unsigned x2, unsigned y2)
{
    auto x = (x1 > x2) ? (x1 - x2) : (x2 - x1);
    auto y = (y1 > y2) ? (y1 - y2) : (y2 - y1);
    return x + y;
}

void common_part(unsigned &min_x, unsigned &min_y, unsigned &max_x, unsigned &max_y, std::vector<std::pair<int, int>> &initial)
{
    min_x = std::numeric_limits<unsigned>::max();
    max_x = std::numeric_limits<unsigned>::min();
    min_y = std::numeric_limits<unsigned>::max();
    max_y = std::numeric_limits<unsigned>::min();
    initial.clear();

    std::ifstream file("input.txt");

    char line = 'A', comma = ',';
    auto x = 0U, y = 0U;

    while (file >> x >> comma >> y)
    {
        auto loc = std::make_pair(y, x);
        initial.push_back(loc);

        grid1[y][x] = line;
        line++;

        min_x = std::min(x, min_x);
        max_x = std::max(x, max_x);
        min_y = std::min(y, min_y);
        max_y = std::max(y, max_y);
    }
}

unsigned do_part1()
{
    auto min_x = 0U, min_y = 0U, max_x = 0U, max_y = 0U;
    std::vector<std::pair<int, int>> initial;
    common_part(min_x, min_y, max_x, max_y, initial);

    for (auto row = min_y; row <= max_y; row++)
        for (auto col = min_x; col <= max_x; col++)
        {
            if (grid1[row][col] >= 'A')
                continue;

            auto closest = std::numeric_limits<unsigned>::max();
            auto closest_letter = '.';

            for (auto &item : initial)
            {
                auto dist = manhattan(item.second, item.first, col, row);
                if (dist < closest)
                {
                    closest = dist;
                    closest_letter = grid1[item.first][item.second];
                }
                else if (dist == closest)
                {
                    closest_letter = '.';
                }
            }

            grid1[row][col] = closest_letter;
        }

    // count area sizes and find those that intersect with outer bounds (infinite expansion)
    std::map<char, unsigned> counts;
    std::set<char> infinite;
    for (auto row = min_y; row <= max_y; row++)
        for (auto col = min_x; col <= max_x; col++)
        {
            auto c = grid1[row][col];
            if (c != '.')
            {
                counts[c]++;
                if (row == min_y || row == max_y || col == min_x || col == max_x)
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
    common_part(min_x, min_y, max_x, max_y, initial);

    auto count = 0U;
    for (auto row = min_y; row <= max_y; row++)
        for (auto col = min_x; col <= max_x; col++)
        {
            auto total_dist = 0;
            for (auto &item : initial)
                total_dist += manhattan(item.second, item.first, col, row);
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
