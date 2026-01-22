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
auto recur(const Grid &grid, int row, int col, LocationSet &reachable_nines)
{
    if (grid[row][col] == '9')
    {
        reachable_nines.insert({ row,col });
        return;
    }

    for (auto &d : dirs)
    {
        auto new_row = row + d.first, new_col = col + d.second;

        if (new_row < 0 || new_row >= grid.size() ||
            new_col < 0 || new_col >= grid[new_row].size())
            continue;

        if (grid[new_row][new_col] == grid[row][col] + 1)
            recur(grid, new_row, new_col, reachable_nines);
    }
}

auto score_trailhead(const Grid &grid, int start_row, int start_col)
{
    LocationSet reachable_nines;
    recur(grid, start_row, start_col, reachable_nines);

    return reachable_nines.size();
}

auto do_part1(const std::string &filename)
{
    // read topology
    std::ifstream file(filename);
    std::string line;
    Grid grid;

    while (file >> line)
        grid.push_back(line);

    // locate trailheads
    LocationList trailheads;
    for (auto row = 0; row < int(grid.size()); row++)
        for (int col = 0; col < int(grid[row].size()); col++)
            if (grid[row][col] == '0')
                trailheads.push_back({ row, col });

    // score each trailhead
    int retval = 0;
    for (auto &th : trailheads)
        retval += score_trailhead(grid, th.first, th.second);

    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 535);

    //auto part2 = do_part2("input.txt");
    //std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == 6379677752410);

    return 0;
}
