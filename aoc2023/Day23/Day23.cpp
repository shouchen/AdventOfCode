#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <cassert>

struct DecisionPoint
{
    int row, col;
};

auto max_path = 0;
const int dr[] = { -1, 1, 0, 0 }, dc[] = { 0, 0, -1 ,1 };

std::vector<DecisionPoint> dps;
std::set<int> seen;
std::vector<std::string> grid;

void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}

auto follow_tunnel(DecisionPoint &dp, int rdir, int cdir, bool respect_slope)
{
    auto prow = dp.row, pcol = dp.col;
    auto row = dp.row + rdir, col = dp.col + cdir;
    auto len = 1;

    for (;;)
    {
        auto found = std::find_if(dps.begin(), dps.end(),
            [row, col](DecisionPoint &dp) {return dp.row == row && dp.col == col; });
        if (found != dps.end())
            return make_pair(found, len);

        auto nrow = -1, ncol = -1;
        for (auto i = 0; i < 4; i++)
        {
            nrow = row + dr[i], ncol = col + dc[i];

            if (respect_slope)
            {
                if (grid[nrow][ncol] == '^' && (dr[i] != -1 || dc[i] != 0)) continue;
                if (grid[nrow][ncol] == 'v' && (dr[i] != 1 || dc[i] != 0)) continue;
                if (grid[nrow][ncol] == '<' && (dr[i] != 0 || dc[i] != -1)) continue;
                if (grid[nrow][ncol] == '>' && (dr[i] != 0 || dc[i] != 1)) continue;
            }

            if (nrow >= 0 && nrow < grid.size() && // don't fall off grid
                grid[nrow][ncol] != '#' && // obstacle
                (nrow != prow || ncol != pcol)) // don't back up
            {
                break;  // no more than one of four should be valid
            }
        }

        if (nrow == -1 && ncol == -1)
            return make_pair(dps.end(), -1); // hit a dead end

        len++;
        prow = row, pcol = col;
        row = nrow, col = ncol;
    }
}

void recur(int dist, int start_index, std::vector<std::vector<int>> &adj)
{
    auto end_index = adj.size() - 1;

    if (start_index == end_index)
    {
        max_path = std::max(max_path, dist);
        return;
    } 

    for (auto i = 0; i < adj[start_index].size(); i++)
        if (adj[start_index][i] && seen.find(i) == seen.end())
        {
            seen.insert(i);
            recur(dist + adj[start_index][i], i, adj);
            seen.erase(i);
        }
}

auto solve(bool respect_slope)
{
    max_path = 0;
    dps.clear();
    seen.clear();

    // 1. Scan grid for decision points (start and end will be first and last ones)
    for (auto i = 0; i < grid.size(); i++)
        for (auto j = 0; j < grid[0].size(); j++)
        {
            if (grid[i][j] == '#')
                continue;

            if (i == 0 || i == grid.size() - 1)
            {
                dps.push_back({ i, j });
                continue;
            }

            auto num_ways = 0;
            for (auto k = 0; k < 4; k++)
                if (grid[i + dr[k]][j + dc[k]] != '#')
                    num_ways++;

            if (num_ways > 2)
                dps.push_back({ i, j });
        }

    // 2. From each decision point, find neighbors and cost (indexes match the dps ones)
    std::vector<std::vector<int>> adj(dps.size(), std::vector<int>(dps.size()));
    for (auto i = 0; i < dps.size(); i++)
        for (auto j = 0; j < 4; j++)
        {
            auto nrow = dps[i].row + dr[j], ncol = dps[i].col + dc[j];
            if (nrow < 0 || nrow == grid.size() || grid[nrow][ncol] == '#')
                continue;

            if (respect_slope)
            {
                if (grid[nrow][ncol] == '^' && (dr[j] != -1 || dc[j] != 0)) continue;
                if (grid[nrow][ncol] == 'v' && (dr[j] != 1 || dc[j] != 0)) continue;
                if (grid[nrow][ncol] == '<' && (dr[j] != 0 || dc[j] != -1)) continue;
                if (grid[nrow][ncol] == '>' && (dr[j] != 0 || dc[j] != 1)) continue;
            }

            auto temp = follow_tunnel(dps[i], dr[j], dc[j], respect_slope);
            if (temp.second != -1)
                adj[i][temp.first - dps.begin()] = temp.second;
        }

    // Do DFS on adjacency matrix, all possible paths
    seen.insert(0); // start dp
    recur(0, 0, adj);

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
