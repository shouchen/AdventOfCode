#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <algorithm>
#include <cassert>

auto tiles = std::vector<std::pair<long long, long long>>();

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    auto x = 0LL, y = 0LL;
    auto comma = ',';

    while (file >> x >> comma >> y)
        tiles.push_back({ x, y });

    auto max_area = 0LL;

    for (auto i = 0; i < tiles.size(); i++)
        for (auto j = 0; j < tiles.size(); j++)
        {
            if (i == j)
                continue;

            auto area = std::abs(tiles[i].first - tiles[j].first + 1) * std::abs(tiles[i].second - tiles[j].second + 1);
            if (area > max_area)
                max_area = area;
        }

    return max_area;
}

auto grid = std::map<std::pair<int, int>, char>();
auto row_min = INT_MAX, row_max = INT_MIN, col_min = INT_MAX, col_max = INT_MIN;

void dump()
{
    for (auto row = row_min - 1; row <= row_max + 1; row++)
    {
        for (auto col = col_min - 1; col <= col_max + 1; col++)
        {
            auto temp = grid.find({ row, col });
            if (temp == grid.end())
                std::cout << ".";
            else
                std::cout << temp->second;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

auto qualify_part2(std::map<std::pair<int, int>, char> &grid, std::pair<int, int> a, std::pair<int, int> b)
{
    auto
        row_start = std::min(a.first, b.first),
        row_end = std::max(a.first, b.first),
        col_start = std::min(a.second, b.second),
        col_end = std::min(a.second, b.second);

    for (auto r = row_start; r <= row_end; r++)
        for (auto c = col_start; c <= col_end; c++)
            if (grid.find({ r, c }) == grid.end())
                return false;

    return true;
}

auto do_part2(const std::string &filename)
{
    auto reds = std::vector<std::pair<int, int>>();

    std::ifstream file(filename);
    auto row = 0, col = 0;
    auto comma = ',';
    std::pair<int, int> origin, previous;

    bool first = true;

    while (file >> col >> comma >> row)
    {
        reds.push_back({ row,  col });

        if (first)
        {
            origin = { row, col };
            first = false;
        }
        else
        {
            if (row == previous.first)
            {
                if (col < previous.second)
                    for (auto i = col + 1; i < previous.second; i++)
                        grid.insert({ { row, i }, 'X' });
                else
                    for (auto i = previous.second+ 1; i < col; i++)
                        grid.insert({ { row, i }, 'X' });
            }
            else
            {
                if (row < previous.first)
                    for (auto i = row + 1; i < previous.first; i++)
                        grid.insert({ { i , col }, 'X' });
                else
                    for (auto i = previous.first + 1; i < row; i++)
                        grid.insert({ { i, col }, 'X' });
            }
        }
        previous = { row, col };

        grid.insert({ { row, col }, '#' });
        col_min = std::min(col, col_min);
        col_max = std::max(col, col_max);
        row_min = std::min(row, row_min);
        row_max = std::max(row, row_max);
    }

    if (origin.first == previous.first)
    {
        if (origin.second < previous.second)
            for (auto i = origin.second + 1; i < previous.second; i++)
                grid.insert({ { origin.first, i }, 'X' });
        else
            for (auto i = previous.second + 1; i < origin.second; i++)
                grid.insert({ { origin.first, i }, 'X' });
    }
    else
    {
        if (origin.first < previous.first)
            for (auto i = origin.first + 1; i < previous.first; i++)
                grid.insert({ { i, origin.second    }, 'X' });
        else
            for (auto i = previous.first + 1; i < row; i++)
                grid.insert({ { i, origin.second }, 'X' });
    }

    // paint interior
    for (auto r = row_min; r <= row_max; r++)
    {
        std::cout << (r / double(row_max - row_min + 1)) * 100 << std::endl;

        auto in_wall = false, interior = false;

        for (auto c = col_min; c <= col_max; c++)
        {
            auto temp = grid.find({ r, c });

            if (in_wall)
            {
                if (temp == grid.end())
                    in_wall = false, interior = !interior;
            }
            else
            {
                if (temp != grid.end())
                    in_wall = true;
            }

            if (interior)
                grid.insert({ { r, c }, 'X' }); 
        }
    }

//    dump();

    auto max_area = 0LL;

    for (auto i = 0; i < reds.size(); i++)
    {
        std::cout << i << std::endl;

        for (auto j = 0; j < reds.size(); j++)
        {
            if (i == j)
                continue;

            if (!qualify_part2(grid, reds[i], reds[j]))
                continue;

            auto area = std::abs(reds[i].first - reds[j].first + 1) * std::abs(reds[i].second - reds[j].second + 1);
            if (area > max_area)
                max_area = area;
        }
    }

    return max_area;
}

int main()
{
    //auto part1 = do_part1("input.txt");
    //std::cout << "Part One: " << part1 << std::endl;
    //assert(part1 == 4782896435);
                                                 
    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
