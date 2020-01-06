#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <cassert>

using XY = std::pair<int, int>;

std::vector<std::string> grid;
auto y_size = 0, x_size = 0;
auto x_origin = 0, y_origin = 0;
auto vapcount = 0;
XY two_hundredth;

auto find_gcf(int a, int b)
{
    while (b > 0)
    {
        auto quotient = a / b;
        auto remainder = a % b;
        a = b;
        b = remainder;
    }

    return a;
}

void reduce_fraction(int &a, int &b)
{
    auto aa = std::abs(a), bb = std::abs(b);
    auto factor = 1;

    if (a == 0)
        factor = bb;
    else if (b == 0)
        factor = aa;
    else
        factor = find_gcf(aa, bb);

    a /= factor, b /= factor;
}

auto count_visible(int x_origin, int y_origin)
{
    auto visible = 0U;

    for (auto y = 0; y < y_size; y++)
        for (auto x = 0; x < x_size; x++)
        {
            if (grid[y][x] != '#')
                continue;

            if (x == x_origin && y == y_origin)
                continue;

            auto delta_x = x - x_origin, delta_y = y - y_origin;
            reduce_fraction(delta_x, delta_y);

            auto currx = x_origin, curry = y_origin;
            for (;;)
            {
                currx += delta_x, curry += delta_y;
                if (currx == x && curry == y)
                {
                    visible++;
                    break;
                }

                if (grid[curry][currx] == '#')
                    break;
            }
        }

    return visible;
}

void vaporize(int delta_x, int delta_y)
{
    auto x = x_origin, y = y_origin;

    for (;;)
    {
        x += delta_x, y += delta_y;
        if (grid[y][x] == '#')
        {
            grid[y][x] = '.';
            if (++vapcount == 200)
                two_hundredth = std::make_pair(x, y);
            break;
        }
    }
}

struct less_than_key
{
    inline bool operator() (const XY &d1, const XY &d2)
    {
        auto a = d1.first ? ((double)d1.second / (double)d1.first) : -1000000;
        auto b = d2.first ? ((double)d2.second / (double)d2.first) : -1000000;

        return a < b;
    }
};

void do_quadrant(int x_start, int x_end, int y_start, int y_end)
{
    std::set<XY> dirns;

    for (auto y = y_start; y <= y_end; y++)
        for (auto x = x_start; x <= x_end; x++)
        {
            if (grid[y][x] != '#')
                continue;

            if (x == x_origin && y == y_origin)
                continue;

            auto delta_x = x - x_origin, delta_y = y - y_origin;
            reduce_fraction(delta_x, delta_y);

            dirns.insert(std::make_pair(delta_x, delta_y));
        }

    std::vector<XY> dirns_sorted;
    for (auto dirn : dirns)
        dirns_sorted.push_back(dirn);

    std::sort(dirns_sorted.begin(), dirns_sorted.end(), less_than_key());

    for (auto curr : dirns_sorted)
        vaporize(curr.first, curr.second);
}

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (getline(file, line))
        grid.push_back(line);

    y_size = grid.size();
    x_size = grid[0].length();
}

auto do_part1()
{
    auto retval = 0;

    for (auto y = 0; y < y_size; y++)
        for (auto x = 0; x < x_size; x++)
        {
            auto v = 0;
            if (grid[y][x] == '#')
                v = count_visible(x, y);

            if (v > retval)
            {
                retval = v;
                x_origin = x, y_origin = y;
            }
        }

    return retval;
}

auto do_part2()
{
    while (vapcount < 200)
    {
        do_quadrant(x_origin, x_size - 1, 0, y_origin - 1);
        do_quadrant(x_origin + 1, x_size - 1, y_origin, y_size - 1);
        do_quadrant(0, x_origin, y_origin + 1, y_size - 1);
        do_quadrant(0, x_origin - 1, 0, y_origin);
    }

    return two_hundredth.first * 100 + two_hundredth.second;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 309);
    assert(part2 == 416);
    return 0;
}
