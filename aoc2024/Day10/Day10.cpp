#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <cassert>

using Coordinates = std::pair<int, int>;
using Grid = std::vector<std::string>;
using CoordinateList = std::vector<Coordinates>;
using CoordinateSet = std::set<Coordinates>;

const std::pair<int, int> dirs[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

auto recur(const Grid &grid, int row, int col, CoordinateSet &reachable_nines, int &num_paths)
{
    if (grid[row][col] == '9')
    {
        reachable_nines.insert({ row, col });
        num_paths++;
        return;
    }

    for (auto &d : dirs)
    {
        auto new_row = row + d.first, new_col = col + d.second;

        if (new_row >= 0 && new_row < grid.size() &&
            new_col >= 0 && new_col < grid[new_row].size() &&
            grid[new_row][new_col] == grid[row][col] + 1)
        {
            recur(grid, new_row, new_col, reachable_nines, num_paths);
        }
    }
}

auto score_and_rate_trail(const Grid &grid, const Coordinates &trailhead)
{
    CoordinateSet reachable_nines;
    auto num_paths = 0;

    recur(grid, trailhead.first, trailhead.second, reachable_nines, num_paths);
    return std::make_pair(reachable_nines.size(), num_paths);
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    Grid grid;

    while (file >> line)
        grid.push_back(line);

    CoordinateList trailheads;
    for (auto row = 0; row < int(grid.size()); row++)
        for (auto col = 0; col < int(grid[row].size()); col++)
            if (grid[row][col] == '0')
                trailheads.push_back(Coordinates(row, col));

    auto retval = std::make_pair(0ULL, 0ULL);
    for (auto &th : trailheads)
    {
        auto score_rating = score_and_rate_trail(grid, th);
        retval.first += score_rating.first;
        retval.second += score_rating.second;
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 535);
    assert(answer.second == 1186);
    return 0;
}
