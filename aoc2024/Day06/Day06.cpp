#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cassert>

using Grid = std::vector <std::string>;
using VisitedMap = std::map<std::pair<int, int>, int>;

auto hash(int row_dir, int col_dir)
{
    return row_dir ? ((row_dir == 1) ? 0x1 : 0x2) : ((col_dir == 1) ? 0x4 : 0x8);
}

auto run_scenario(Grid &grid, int guard_row, int guard_col, VisitedMap &visited)
{
    auto row_dir = -1, col_dir = 0;
    visited.clear();

    for (;;)
    {
        auto guard_pos = std::make_pair(guard_row, guard_col);
        auto map_it = visited.find(guard_pos);

        if (map_it != visited.end() && map_it->second & hash(row_dir, col_dir))
            return false;

        visited[guard_pos] |= hash(row_dir, col_dir);

        auto new_row = guard_row + row_dir, new_col = guard_col + col_dir;
        if (new_row < 0 || new_row >= int(grid.size()) || new_col < 0 || new_col >= int(grid[0].size()))
            return true;

        if (grid[new_row][new_col] == '#')
        {
            std::swap(row_dir, col_dir);
            col_dir = -col_dir;
        }
        else
        {
            guard_row += row_dir, guard_col += col_dir;
        }
    }
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    Grid grid;
    auto guard_row = 0, guard_col = 0;

    while (std::getline(file, line))
    {
        auto pos = line.find('^');
        if (pos != std::string::npos)
            guard_row = int(grid.size()), guard_col = int(pos);

        grid.push_back(line);
    }

    std::pair<int, int> retval;
    VisitedMap visited_part1, visited_part2;

    run_scenario(grid, guard_row, guard_col, visited_part1);
    retval.first = int(visited_part1.size());

    for (const auto &v : visited_part1)
    {
        auto obs_row = v.first.first, obs_col = v.first.second;

        grid[obs_row][obs_col] = '#';
        if (!run_scenario(grid, guard_row, guard_col, visited_part2))
            retval.second++;

        grid[obs_row][obs_col] = '.';
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 4967);
    assert(answer.second == 1789);
    return 0;
}
