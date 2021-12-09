#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <cassert>

std::vector<std::string> grid;
std::set<std::pair<int, int>> visited;

auto count_basin(int row, int col)
{
    auto pair = std::make_pair(row, col);

    if (row < 0 || row > int(grid.size() - 1) || col < 0 || col > int(grid.size() - 1) ||
        visited.find(pair) != visited.end() ||  grid[row][col] == '9')
        return 0;

    visited.insert(pair);
    return 1 + count_basin(row - 1, col) + count_basin(row + 1, col) +
               count_basin(row, col - 1) + count_basin(row, col + 1);
}

auto do_parts(const std::string &filename)
{
    std::set<std::pair<int, int>> low_points;
    std::ifstream file(filename);
    std::string line;
    auto part1 = 0;

    while (file >> line)
        grid.push_back(line);

    for (auto row = 0U; row < grid.size(); row++)
    {
        for (auto col = 0U; col < grid[0].size(); col++)
        {
            auto curr = grid[row][col];

            if ((row == 0 || grid[row - 1][col] > curr) &&
                (row == grid.size() - 1 || grid[row + 1][col] > curr) &&
                (col == 0 || grid[row][col - 1] > curr) &&
                (col == grid[0].size() - 1 || grid[row][col + 1] > curr))
            {
                low_points.insert(std::make_pair(row, col));
                part1 += curr - '0' + 1;
            }
        }
    }

    std::vector<int> basin_sizes;
    for (auto lp : low_points)
        basin_sizes.push_back(count_basin(lp.first, lp.second));

    std::sort(basin_sizes.begin(), basin_sizes.end());
    auto last = basin_sizes.size() - 1;
    auto part2 = basin_sizes[last] * basin_sizes[last - 1] * basin_sizes[last - 2];
    return std::make_pair(part1, part2);
}

int main()
{
    auto result = do_parts("input.txt");

    std::cout << "Part One: " << result.first << std::endl;
    std::cout << "Part Two: " << result.second << std::endl;

    assert(result.first == 512);
    assert(result.second == 1600104);
    return 0;
}
