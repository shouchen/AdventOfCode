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
std::set<std::pair<int, int>> seen;
const int dr[] = { -1, 1, 0, 0 }, dc[] = { 0, 0, -1 ,1 };

std::vector<DecisionPoint> dps;
std::set<int> seen2;
std::vector<std::string> grid;

void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}

void recur1(int steps, int row, int col)
{
    if (row == grid.size() - 1)
    {
        if (steps > max_path)
            max_path = steps;

        return;
    }

    if (row < 0 || seen.find(std::make_pair(row, col)) != seen.end())
        return;

    auto inserted = seen.insert(std::make_pair(row, col));

    for (auto i = 0; i < 4; i++)
    {
        auto nrow = row + dr[i], ncol = col + dc[i];

        if (nrow >= 0 && nrow < grid.size())
        {
            if (nrow < grid.size())
            {
                if (grid[nrow][ncol] == '#') continue;
                if (grid[nrow][ncol] == '^' && (dr[i] != -1 || dc[i] != 0)) continue;
                if (grid[nrow][ncol] == 'v' && (dr[i] != 1 || dc[i] != 0)) continue;
                if (grid[nrow][ncol] == '<' && (dr[i] != 0 || dc[i] != -1)) continue;
                if (grid[nrow][ncol] == '>' && (dr[i] != 0 || dc[i] != 1)) continue;
            }

            recur1(steps + 1, nrow, ncol);
        }
    }

    auto found = seen.find(std::make_pair(row, col));
    seen.erase(found);
}

auto follow_tunnel(DecisionPoint &dp, int rdir, int cdir)
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

void recur2(int dist, int start_index, std::vector<std::vector<int>> &adj)
{
    auto end_index = adj.size() - 1;

    if (start_index == end_index)
    {
        max_path = std::max(max_path, dist);
        return;
    } 

    for (auto i = 0; i < adj[start_index].size(); i++)
        if (adj[start_index][i] && seen2.find(i) == seen2.end())
        {
            seen2.insert(i);
            recur2(dist + adj[start_index][i], i, adj);
            seen2.erase(i);
        }
}

auto do_part1()
{
    max_path = 0;
    seen.clear();
    recur1(0, 0, 1);
    return max_path;
}

auto do_part2()
{
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

            auto temp = follow_tunnel(dps[i], dr[j], dc[j]);
            if (temp.second != -1)
                adj[i][temp.first - dps.begin()] = temp.second;
        }

    // Do DFS on adjacency matrix, all possible paths
    max_path = 0;
    seen2.insert(0); // start dp
    recur2(0, 0, adj);

    return max_path;
}

int main()
{
    read_grid("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 2154);
    assert(part2 == 6654);
    return 0;
}
