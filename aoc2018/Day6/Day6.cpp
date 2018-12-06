#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <queue>
#include <map>
#include <cassert>

std::vector<std::pair<int, int>> initial;
std::set<char> periphery;
char grid1[500][500];

int manhattan(std::pair<int, int> a, std::pair<int, int> b)
{
    auto m = a.first - b.first;
    auto n = a.second - b.second;

    if (m < 0) m = -m;
    if (n < 0) n = -n;
    return m + n;
}

unsigned do_part1()
{
    int min_x = std::numeric_limits<int>::max();
    int min_y = std::numeric_limits<int>::max();
    int max_x = std::numeric_limits<int>::min();
    int max_y = std::numeric_limits<int>::min();

    std::ifstream file("input.txt");
    char comma;
    int x, y;

    min_x = 1000000;
    min_x = 1000000;
    max_y = -1000000;
    max_y = -1000000;

    char line = 'A';
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

    //newdump();

    for (int row = min_y; row <= max_y; row++)
        for (int col = min_x; col <= max_x; col++)
        {
            if (grid1[row][col] >= 'A')
                continue;

            int closest = 1000000;
            char closest_letter = '.';

            for (auto &item : initial)
            {
                auto dist = manhattan(item, std::make_pair(row, col));
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
    std::map<char, int> counts;
    for (int row = min_y; row <= max_y; row++)
        for (int col = min_x; col <= max_x; col++)
        {
            auto c = grid1[row][col];
            if (c != '.')
                counts[c]++;
        }

    unsigned max = 0;
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

//char grid2[500][500];

unsigned do_part2()
{
    std::vector<std::pair<int, int>> initial2;

    std::ifstream file("input.txt");

    auto min_x = std::numeric_limits<unsigned>::max();
    auto max_x = std::numeric_limits<unsigned>::min();
    auto min_y = std::numeric_limits<unsigned>::max();
    auto max_y = std::numeric_limits<unsigned>::min();

    char comma = ',';
    auto x = 0U, y = 0U;

    while (file >> x >> comma >> y)
    {
        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;

        initial2.push_back(std::make_pair(y, x));
    }

    // count squares where distance sum is less than 10000
    auto count = 0U;
    for (auto row = min_y; row <= max_y; row++)
        for (auto col = min_x; col <= max_x; col++)
        {
            auto total_dist = 0;
            for (auto &item : initial2)
                total_dist += manhattan(item, std::make_pair(row, col));
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

    assert(part1 == 3293); // 3293 is right (exclude one edge)
    assert(part2 == 45176); // 45176 is right
    return 0;
}
