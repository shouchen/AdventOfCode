#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

using Coordinates = std::pair<int, int>;

auto read_input(const std::string &filename, Coordinates &top_left, Coordinates &bottom_right)
{
    std::ifstream file(filename);
    auto x = 0, y = 0;
    auto comma = ',';
    std::map<Coordinates, int> retval;
    auto area = 0U;

    top_left = std::make_pair(std::numeric_limits<unsigned>::max(), std::numeric_limits<unsigned>::max());
    bottom_right = std::make_pair(std::numeric_limits<unsigned>::min(), std::numeric_limits<unsigned>::min());

    while (file >> x >> comma >> y)
    {
        retval[std::make_pair(x, y)] = ++area;

        top_left = { std::min(x, top_left.first), std::min(y, top_left.second) };
        bottom_right = { std::max(x, bottom_right.first), std::max(y, bottom_right.second) };
    }

    return retval;
}

auto get_manhattan_dist(const Coordinates &c1, const Coordinates &c2)
{
    return static_cast<unsigned>(abs(c1.first - c2.first) + abs(c1.second - c2.second));
}

auto get_closest_area(const Coordinates &loc, std::map<Coordinates, int> &grid)
{
    auto closest = std::numeric_limits<unsigned>::max();
    auto closest_area = -1;

    for (auto &item : grid)
    {
        auto dist = get_manhattan_dist(item.first, loc);

        if (dist < closest)
        {
            closest = dist;
            closest_area = item.second;
        }
        else if (dist == closest)
        {
            closest_area = -1;
        }
    }

    return closest_area;
}

auto do_part1(const std::string &filename)
{
    Coordinates top_left, bottom_right;
    auto input = read_input(filename, top_left, bottom_right);

    std::map<unsigned, unsigned> area_counts;
    std::set<unsigned> infinite_areas;

    for (auto y = top_left.second; y <= bottom_right.second; y++)
        for (auto x = top_left.first; x <= bottom_right.first; x++)
        {
            auto a = get_closest_area(std::make_pair(x, y), input);

            if (a != -1)
            {
                area_counts[a]++;
                if (y == top_left.second || y == bottom_right.second || x == top_left.first || x == bottom_right.first)
                    infinite_areas.insert(a);
            }
        }

    auto max_finite = 0U;
    for (auto &item : area_counts)
        if (item.second > max_finite && infinite_areas.find(item.first) == infinite_areas.end())
            max_finite = item.second;

    return max_finite;
}

auto do_part2(const std::string &filename)
{
    Coordinates top_left, bottom_right;
    auto input = read_input(filename, top_left, bottom_right);

    auto count = 0U;
    for (auto y = top_left.second; y <= bottom_right.second; y++)
        for (auto x = top_left.first; x <= bottom_right.first; x++)
        {
            auto dist_from_all_areas = 0;
            for (auto &item : input)
                dist_from_all_areas += get_manhattan_dist(item.first, std::make_pair(x, y));

            if (dist_from_all_areas < 10000)
                count++;
        }

    return count;
}

int main()
{
    auto part1 = do_part1("input.txt");
    auto part2 = do_part2("input.txt");

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 3293);
    assert(part2 == 45176);
    return 0;
}
