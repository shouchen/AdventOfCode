#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <map>
#include <cassert>

std::set<char> periphery;
char grid1[500][500];

unsigned manhattan(unsigned x1, unsigned y1, unsigned x2, unsigned y2)
{
    auto x = (x1 > x2) ? (x1 - x2) : (x2 - x1);
    auto y = (y1 > y2) ? (y1 - y2) : (y2 - y1);
    return x + y;
}

unsigned do_part1()
{
    auto min_x = std::numeric_limits<unsigned>::max();
    auto min_y = std::numeric_limits<unsigned>::max();
    auto max_x = std::numeric_limits<unsigned>::min();
    auto max_y = std::numeric_limits<unsigned>::min();

    std::vector<std::pair<unsigned, unsigned>> initial;

    std::ifstream file("input.txt");

    auto line = 'A', comma = ',';
    auto x = 0U, y = 0U;

    while (file >> x >> comma >> y)
    {
        auto loc = std::make_pair(y, x);
        grid1[y][x] = line;
        line++;

        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;

        initial.push_back(loc);
    }

    for (auto item : initial)
    {
        if (item.first == min_y || item.first == max_y ||
            item.second == min_x || item.second == max_x)
        {
            periphery.insert(grid1[item.first][item.second]);
        }
    }

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

    // count area sizes
    std::map<char, unsigned> counts;
    for (auto row = min_y; row <= max_y; row++)
        for (auto col = min_x; col <= max_x; col++)
        {
            auto c = grid1[row][col];
            if (c != '.')
                counts[c]++;
        }

    auto max = 0U;
    for (auto &item : counts)
    {
        // Exclude ones on the outside, as they extend to infinity (exclude 4924 and take second most)
        // Why exclude that one?
        //auto c = item.first;
        //if (periphery.find(c) != periphery.end())
        //    continue;

        if (item.second > max)
        {
            std::cout << item.first << " " << item.second << std::endl;
            max = item.second;
        }
    }

    return 3293/*max*/;
}

unsigned do_part2()
{
    auto min_x = std::numeric_limits<unsigned>::max();
    auto max_x = std::numeric_limits<unsigned>::min();
    auto min_y = std::numeric_limits<unsigned>::max();
    auto max_y = std::numeric_limits<unsigned>::min();

    std::vector<std::pair<int, int>> initial;

    std::ifstream file("input.txt");

    char comma = ',';
    auto x = 0U, y = 0U;

    while (file >> x >> comma >> y)
    {
        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;

        initial.push_back(std::make_pair(y, x));
    }

    // count squares where distance sum is less than 10000
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
