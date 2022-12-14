#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cassert>

std::map<std::pair<int, int>, char> grid;
auto min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;

auto do_part1(const std::string &filename)
{
    grid.clear();
    min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;

    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::pair<int, int> from, to;
        std::string arrow;
        auto comma = ',';

        ss >> from.first >> comma >> from.second;
        while (ss >> arrow >> to.first >> comma >> to.second)
        {
            if (from.first == to.first)
            {
                if (from.second < to.second)
                    for (auto i = from.second; i <= to.second; i++)
                        grid[std::make_pair(from.first, i)] = '#';
                else
                    for (auto i = from.second; i >= to.second; i--)
                        grid[std::make_pair(from.first, i)] = '#';
            }
            else
            {
                if (from.first < to.first)
                    for (auto i = from.first; i <= to.first; i++)
                        grid[std::make_pair(i, from.second)] = '#';
                else
                    for (auto i = from.first; i >= to.first; i--)
                        grid[std::make_pair(i, from.second)] = '#';
            }

            from = to;
        }
    }

    for (auto &i : grid)
    {
        min_x = std::min(min_x, i.first.first);
        max_x = std::max(max_x, i.first.first);
        min_y = std::min(min_y, i.first.second);
        max_y = std::max(max_y, i.first.second);
    }

    auto units = 0;
    for (;;)
    {
        auto sand = std::make_pair(500, 0);
        for (;;)
        {
            if (grid.find(std::make_pair(sand.first, sand.second + 1)) == grid.end())
                sand.second++;
            else if (grid.find(std::make_pair(sand.first - 1, sand.second + 1)) == grid.end())
                sand.first--, sand.second++;
            else if (grid.find(std::make_pair(sand.first + 1, sand.second + 1)) == grid.end())
                sand.first++, sand.second++;
            else
                break;

            if (sand.first < min_x || sand.first > max_x)
                return units;
        }

        grid[sand] = 'o';
        units++;
    }
}

auto do_part2(const std::string &filename)
{
    grid.clear();
    min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;

    std::ifstream file(filename);
    std::string line;
    std::pair<int, int> from, to;

    while (getline(file, line))
    {
        std::stringstream ss(line);
        int a, b, c, d;
        std::string arrow;
        auto comma = ',';

        ss >> from.first >> comma >> from.second;
        while (ss >> arrow >> to.first >> comma >> to.second)
        {
            if (from.first == to.first)
            {
                if (from.second < to.second)
                    for (auto i = from.second; i <= to.second; i++)
                        grid[std::make_pair(from.first, i)] = '#';
                else
                    for (auto i = from.second; i >= to.second; i--)
                        grid[std::make_pair(from.first, i)] = '#';
            }
            else
            {
                if (from.first < to.first)
                    for (auto i = from.first; i <= to.first; i++)
                        grid[std::make_pair(i, from.second)] = '#';
                else
                    for (auto i = from.first; i >= to.first; i--)
                        grid[std::make_pair(i, from.second)] = '#';
            }

            from = to;
        }
    }

    for (auto &i : grid)
    {
        min_x = std::min(min_x, i.first.first);
        max_x = std::max(max_x, i.first.first);
        min_y = std::min(min_y, i.first.second);
        max_y = std::max(max_y, i.first.second);
    }

    for (auto i = -10000; i <= 10000; i++)
        grid[std::make_pair(i, max_y + 2)] = '#';

    auto units = 0;
    for (;;)
    {
        auto sand = std::make_pair(500, 0);
        for (;;)
        {
            if (grid.find(std::make_pair(sand.first, sand.second + 1)) == grid.end())
                sand.second++;
            else if (grid.find(std::make_pair(sand.first - 1, sand.second + 1)) == grid.end())
                sand.first--, sand.second++;
            else if (grid.find(std::make_pair(sand.first + 1, sand.second + 1)) == grid.end())
                sand.first++, sand.second++;
            else
                break;
        }

        grid[sand] = 'o';
        units++;

        if (sand.first == 500 && sand.second == 0)
            return units;
    }
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 901);
    assert(part2 == 24589);
    return 0;
}
