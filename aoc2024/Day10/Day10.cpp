#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <cassert>

using Grid = std::vector<std::string>;
using LocationList = std::vector<std::pair<int, int>>;
using LocationSet = std::set<std::pair<int, int>>;
using DirectionList = std::vector<std::pair<int, int>>;

const DirectionList dirs = { {-1,0}, {1,0}, {0,-1}, {0,1} };

// DFS - builds up the location set
auto recur(const Grid &grid, int row, int col, LocationSet &reachable_nines, unsigned long long &num_paths)
{
    if (grid[row][col] == '9')
    {
        reachable_nines.insert({ row,col });
        num_paths++;
        return;
    }

    for (auto &d : dirs)
    {
        auto new_row = row + d.first, new_col = col + d.second;

        if (new_row < 0 || new_row >= grid.size() ||
            new_col < 0 || new_col >= grid[new_row].size())
            continue;

        if (grid[new_row][new_col] == grid[row][col] + 1)
            recur(grid, new_row, new_col, reachable_nines, num_paths);
    }
}

// first = score, second = rating
auto score_trailhead(const Grid &grid, int start_row, int start_col)
{
    LocationSet reachable_nines;
    auto num_paths = 0ULL;

    recur(grid, start_row, start_col, reachable_nines, num_paths);

    return std::make_pair(reachable_nines.size(), num_paths);
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    Grid grid;

    while (file >> line)
        grid.push_back(line);

    LocationList trailheads;
    for (auto row = 0; row < int(grid.size()); row++)
        for (int col = 0; col < int(grid[row].size()); col++)
            if (grid[row][col] == '0')
                trailheads.push_back({ row, col });

    auto retval = std::make_pair(0ULL, 0ULL);
    for (auto &th : trailheads)
    {
        auto temp = score_trailhead(grid, th.first, th.second);
        retval.first += temp.first;
        retval.second += temp.second;
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
