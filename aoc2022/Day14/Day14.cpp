#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cassert>

std::map<std::pair<int, int>, char> grid;
auto max_y = INT_MIN;
auto source = std::make_pair(500, 0);

void read_input(const std::string &filename)
{
    grid.clear();
    max_y = INT_MIN;

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
        max_y = std::max(max_y, i.first.second);
}

auto do_part1(const std::string &filename)
{
    read_input(filename);
    auto units = 0;

    for (;;)
    {
        auto sand = source;
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

            if (sand.second == max_y + 1)
                return units;
        }

        grid[sand] = 'o';
        units++;
    }
}

auto do_part2(const std::string &filename)
{
    read_input(filename);
    auto units = 0;

    for (;;)
    {
        auto sand = source;
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

            if (sand.second == max_y + 1)
                break;
        }

        grid[sand] = 'o';
        units++;

        if (sand == source)
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
