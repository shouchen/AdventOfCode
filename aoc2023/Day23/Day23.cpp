#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

using Point = std::pair<int, int>;
using Points = std::vector<Point>;
using Grid = std::vector<std::string>;

const int dr[] = { -1, 1, 0, 0 }, dc[] = { 0, 0, -1 ,1 };
const std::string anti_slope = "v^><";

auto read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    Grid grid;

    while (std::getline(file, line))
        grid.push_back(line);

    return grid;
}

auto can_advance(Grid &grid, int row, int col, bool respect_slope, int dir_index)
{
    return row >= 0 && row < grid.size() && grid[row][col] != '#' &&
        (!respect_slope || anti_slope.find(grid[row][col]) != dir_index);
}

auto find_forks(Grid &grid)
{
    std::vector<std::pair<int,int>> retval;

    for (auto i = 0; i < grid.size(); i++)
        for (auto j = 0; j < grid[0].size(); j++)
        {
            if (grid[i][j] == '#')
                continue;

            if (i == 0 || i == grid.size() - 1)
            {
                retval.push_back({ i, j });
                continue;
            }

            auto num_ways = 0;
            for (auto k = 0; k < 4; k++)
                if (grid[i + dr[k]][j + dc[k]] != '#')
                    num_ways++;

            if (num_ways > 2)
                retval.push_back({ i, j });
        }

    return retval;
}

auto follow_tunnel(Grid &grid, Points &forks, int index, int index_dir, bool respect_slope)
{
    Point &fork = forks[index];
    auto prev_row = fork.first, prev_col = fork.second;
    auto row = fork.first + dr[index_dir], col = fork.second + dc[index_dir];

    for (auto len = 1; ; len++)
    {
        auto found = std::find_if(forks.begin(), forks.end(),
            [row, col](Point &fork) {return fork.first == row && fork.second == col; });
        if (found != forks.end())
            return make_pair(found, len);

        auto next_row = -1, next_col = -1;
        for (auto i = 0; i < 4; i++)
        {
            next_row = row + dr[i], next_col = col + dc[i];

            if ((next_row != prev_row || next_col != prev_col) && can_advance(grid, next_row, next_col, respect_slope, i))
                break;
        }

        if (next_row == -1 && next_col == -1)
            return make_pair(forks.end(), -1);

        prev_row = row, prev_col = col;
        row = next_row, col = next_col;
    }
}

auto build_adjacency_matrix(Grid &grid, Points &forks, bool respect_slope)
{
    std::vector<std::vector<int>> adj(forks.size(), std::vector<int>(forks.size()));

    for (auto i = 0; i < forks.size(); i++)
        for (auto j = 0; j < 4; j++)
        {
            auto row = forks[i].first + dr[j], col = forks[i].second + dc[j];

            if (!can_advance(grid, forks[i].first + dr[j], forks[i].second + dc[j], respect_slope, j))
                continue;

            auto tunnel_end = follow_tunnel(grid, forks, i, j, respect_slope);
            if (tunnel_end.second != -1)
                adj[i][tunnel_end.first - forks.begin()] = tunnel_end.second;
        }

    return adj;
}

auto solve(Grid &grid, bool respect_slope)
{
    auto forks = find_forks(grid);
    auto adj = build_adjacency_matrix(grid, forks, respect_slope);
    auto max_path = 0;

    auto seen = std::vector<bool>(forks.size());
    seen[0] = true;

    auto recur = [&](auto &self, int dist, int start_index)
    {
        if (start_index == adj.size() - 1)
        {
            max_path = std::max(max_path, dist);
            return;
        }

        for (auto i = 0; i < adj[start_index].size(); i++)
            if (adj[start_index][i] && !seen[i])
            {
                seen[i] = true;
                self(self, dist + adj[start_index][i], i);
                seen[i] = false;
            }
    };

    recur(recur, 0, 0);
    return max_path;
}

int main()
{
    auto grid = read_grid("input.txt");

    auto part1 = solve(grid, true);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = solve(grid, false);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 2154);
    assert(part2 == 6654);
    return 0;
}
