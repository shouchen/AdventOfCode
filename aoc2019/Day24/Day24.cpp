#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <array>
#include <set>
#include <cassert>

using XYZ = std::tuple<int, int, int>;
using Grid3D = std::set<XYZ>;
Grid3D space;

auto has_bug(int row, int col, int level)
{
    return space.find(std::make_tuple(row, col, level)) != space.end();
}

auto count_adjacent(int row, int col, int level, int row_dir, int col_dir, bool part2)
{
    auto new_row = row + row_dir, new_col = col + col_dir, new_level = level;

    if (part2)
    {
        // part2 going into middle (check 5 adjacents)
        if (new_row == 2 && new_col == 2)
        {
            auto row_inc = 0, col_inc = 0;
            auto adj = 0U;

            if (row_dir == 0)
                new_row = 0, new_col = (col_dir == -1) ? 4 : 0, row_inc = 1;
            else
                new_row = (row_dir == -1) ? 4 : 0, new_col = 0, col_inc = 1;

            new_level++;

            for (int i = 0; i < 5; i++, new_row += row_inc, new_col += col_inc)
                if (has_bug(new_row, new_col, new_level))
                    adj++;

            return adj;
        }

        // part2 going out of edges (modify the one adjacent to check)
        if (new_row < 0 || new_row == 5 || new_col < 0 || new_col == 5)
            new_row = 2 + row_dir, new_col = 2 + col_dir, new_level = level - 1;
    }

    // check one adjacent
    return has_bug(new_row, new_col, new_level) ? 1U : 0U;
}

auto count_adjacent(int row, int col, int level = 0, bool part2 = false)
{
    return
        count_adjacent(row, col, level, -1, 0, part2) +
        count_adjacent(row, col, level, 1, 0, part2) +
        count_adjacent(row, col, level, 0, -1, part2) +
        count_adjacent(row, col, level, 0, 1, part2);
}

// pass in min, max levels to evaluate; outvals are the actual min and max levels
auto generate(int &min_level, int &max_level, bool part2)
{
    Grid3D retval;
    auto next_min_level = INT_MAX, next_max_level = INT_MIN;

    for (auto level = min_level; level <= max_level; level++)
    {
        for (auto i = 0; i < 5; i++)
        {
            for (auto j = 0; j < 5; j++)
            {
                if (part2 && i == 2 && j == 2)
                    continue;

                auto adj = count_adjacent(i, j, level, part2);
                auto has_bug = space.find(std::make_tuple(i, j, level)) != space.end();

                if ((has_bug && adj == 1) || (!has_bug && (adj == 1 || adj == 2)))
                {
                    retval.insert(std::make_tuple(i, j, level));
                    next_min_level = std::min(next_min_level, level);
                    next_max_level = std::max(next_max_level, level);
                }
            }
        }
    }

    min_level = next_min_level;
    max_level = next_max_level;
    return retval;
}

auto read_input(const std::string &filename)
{
    Grid3D retval;
    std::ifstream file(filename);
    auto c = ' ';

    for (auto i = 0; i < 5; i++)
        for (auto j = 0; j < 5; j++)
        {
            file >> c;
            if (c == '#')
                retval.insert(std::make_tuple(i, j, 0));
        }

    return retval;
}

auto do_part1()
{
    space = read_input("input.txt");
    std::set<Grid3D> seen;

    for (;;)
    {
        auto min_level = 0, max_level = 0;
        space = generate(min_level, max_level, false);

        if (seen.find(space) != seen.end())
            break;

        seen.insert(space);
    }

    auto p = 1LL, sum = 0LL;
    for (auto i = 0; i < 5; i++)
        for (auto j = 0; j < 5; j++)
        {
            if (has_bug(i, j, 0))
                sum += p;
            p <<= 1;
        }

    return sum;
}

unsigned do_part2()
{
    space = read_input("input.txt");

    auto min_level = 0, max_level = 0;
    for (auto i = 0; i < 200; i++)
        space = generate(--min_level, ++max_level, true);

    return space.size();
}

int main()
{
    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 18852849);
    assert(part2 == 1948);
    return 0;
}
