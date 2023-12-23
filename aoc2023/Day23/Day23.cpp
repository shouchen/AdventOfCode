#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

using Point = std::pair<int, int>;
using Points = std::vector<Point>;

const int dr[] = { -1, 1, 0, 0 }, dc[] = { 0, 0, -1 ,1 };
const std::string anti_slope = "v^><";
std::vector<std::string> grid;

void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}

auto can_advance(int row, int col, bool respect_slope, int dir_index)
{
    return row >= 0 && row < grid.size() && grid[row][col] != '#' &&
        (!respect_slope || anti_slope.find(grid[row][col]) != dir_index);
}

auto find_forks()
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

auto follow_tunnel(Points &forks, int index, int rdir, int cdir, bool respect_slope)
{
    Point &fork = forks[index];
    auto prow = fork.first, pcol = fork.second;
    auto row = fork.first + rdir, col = fork.second + cdir;

    for (auto len = 1; ; len++)
    {
        auto found = std::find_if(forks.begin(), forks.end(),
            [row, col](Point &fork) {return fork.first == row && fork.second == col; });
        if (found != forks.end())
            return make_pair(found, len);

        auto nrow = -1, ncol = -1;
        for (auto i = 0; i < 4; i++)
        {
            nrow = row + dr[i], ncol = col + dc[i];

            if ((nrow != prow || ncol != pcol) && can_advance(nrow, ncol, respect_slope, i))
                break;
        }

        if (nrow == -1 && ncol == -1)
            return make_pair(forks.end(), -1);

        prow = row, pcol = col;
        row = nrow, col = ncol;
    }
}

auto build_adjacency_matrix(Points &forks, bool respect_slope)
{
    std::vector<std::vector<int>> adj(forks.size(), std::vector<int>(forks.size()));

    for (auto i = 0; i < forks.size(); i++)
        for (auto j = 0; j < 4; j++)
        {
            auto row = forks[i].first + dr[j], col = forks[i].second + dc[j];

            if (can_advance(row, col, respect_slope, j))
            {
                auto tunnel_end = follow_tunnel(forks, i, dr[j], dc[j], respect_slope);
                if (tunnel_end.second != -1)
                    adj[i][tunnel_end.first - forks.begin()] = tunnel_end.second;
            }
        }

    return adj;
}

void recur(int dist, int start_index, std::vector<std::vector<int>> &adj, std::vector<bool> &seen, int &max_path)
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
            recur(dist + adj[start_index][i], i, adj, seen, max_path);
            seen[i] = false;
        }
}

auto solve(bool respect_slope)
{
    auto forks = find_forks();
    auto adj = build_adjacency_matrix(forks, respect_slope);
    auto seen = std::vector<bool>(forks.size());
    seen[0] = true;
    auto max_path = 0;

    recur(0, 0, adj, seen, max_path);
    return max_path;
}

int main()
{
    read_grid("input.txt");

    auto part1 = solve(true);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = solve(false);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 2154);
    assert(part2 == 6654);
    return 0;
}
// TODO: Make recur return max path instead of passing as param
// TODO: Use nested, recursive lambda to avoid so much context passing