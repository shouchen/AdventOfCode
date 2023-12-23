#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <ctype.h>
#include <algorithm>
#include <numeric>
#include <regex>
#include <cassert>

using namespace std;

std::vector<std::string> grid;
void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}
void dump_grid()
{
    for (auto &row : grid)
        std::cout << row << std::endl;
}

long long max_path = 0;
set<pair<int, int>> seen;

void recur(int steps, int row, int col)
{
    static const int dr[] = { -1, 1,0,0 }, dc[] = { 0,0,-1,1 };

    if (row == grid.size() - 1)
    {
        if (steps > max_path)
        {
            max_path = steps;
            cout << "New max = " << max_path << endl;
        }
        return;
    }
    if (row < 0)
        return;

    if (seen.find(make_pair(row, col)) != seen.end())
    {
        return;
    }
    auto inserted = seen.insert(make_pair(row, col));

    for (int i = 0; i < 4; i++)
    {
        int nrow = row + dr[i], ncol = col + dc[i];

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

            recur(steps + 1, nrow, ncol);
        }
    }

    auto found = seen.find(make_pair(row, col));
    assert(found != seen.end());

    seen.erase(found);
}

void recur2(int steps, int row, int col)
{
    static const int dr[] = { -1, 1,0,0 }, dc[] = { 0,0,-1,1 };

    if (row == grid.size() - 1)
    {
        if (steps > max_path)
        {
            max_path = steps;
            cout << "New max = " << max_path << endl;
        }
        return;
    }
    if (row < 0)
        return;

    if (seen.find(make_pair(row, col)) != seen.end())
    {
        return;
    }
    auto inserted = seen.insert(make_pair(row, col));

    for (int i = 0; i < 4; i++)
    {
        int nrow = row + dr[i], ncol = col + dc[i];

        if (nrow >= 0 && nrow < grid.size())
        {
            if (nrow < grid.size())
            {
                if (grid[nrow][ncol] == '#') continue;
            }

            recur2(steps + 1, nrow, ncol);
        }
    }

    auto found = seen.find(make_pair(row, col));
    assert(found != seen.end());

    seen.erase(found);
}

auto do_part1()
{
    max_path = 0;
    seen.clear();
    recur(0, 0, 1);
    return max_path;
}

auto do_part2()
{
    max_path = 0;
    seen.clear();
    recur2(0, 0, 1);
    return max_path;
}

struct DecisionPoint
{
    int row, col;
};

vector<DecisionPoint> dps;
const int dr[] = { -1, 1,0,0 }, dc[] = { 0,0,-1,1 };

auto follow_tunnel(DecisionPoint &dp, int rdir, int cdir)
{
    int prow = dp.row, pcol = dp.col;
    int row = dp.row + rdir, col = dp.col + cdir;
    int len = 1;

    for (;;)
    {
        auto found = std::find_if(dps.begin(), dps.end(),
            [row, col](DecisionPoint &dp) {return dp.row == row && dp.col == col; });
        if (found != dps.end())
        {
            return make_pair(found, len);
        }

        int nrow = -1, ncol = -1;
        for (int i = 0; i < 4; i++) // no more than one of four should be valid
        {
            nrow = row + dr[i], ncol = col + dc[i];
            if (nrow < 0 || nrow == grid.size()) continue; // don't fall off grid
            if (grid[nrow][ncol] == '#') continue; // obstacle
            if (nrow == prow && ncol == pcol) continue; // don't back up
            break;
        }

        if (nrow == -1 && ncol == -1)
            return make_pair(dps.end(), -1); // hit a dead end

        len++;

        prow = row, pcol = col;
        row = nrow, col = ncol;
    }
}

int minDistance(vector<int> &dist, vector<bool> &sptSet)
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < dps.size(); v++)
        if (!sptSet[v] && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

auto dijkstra(vector<vector<int>> graph, int src, int dest) // single source, single dest
{
    vector<int> dist(graph.size(), INT_MAX);
    vector<bool> sptSet(graph.size(), false);

    dist[src] = 0;

    for (int count = 0; count < dps.size() - 1; count++)
    {
        int u = minDistance(dist, sptSet);

        sptSet[u] = true;

        for (int v = 0; v < dps.size(); v++)
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v];
            }
    }

    //debug
    cout << "Distances: " << endl;
    for (int i = 0; i < dist.size(); i++)
    {
        cout << i << ": " << dist[i] << endl;
    }

    return dist[dest];
}

int max_path4 = 0;
set<int> seen4;

void recur4(int dist, int start_index, int end_index, vector<vector<int>> &adj)
{
    if (start_index == end_index)
    {
        max_path4 = max(max_path4, dist);
        return;
    } 

    for (int i = 0; i < adj[start_index].size(); i++)
    {
        if (adj[start_index][i] && seen4.find(i) == seen4.end())
        {
            seen4.insert(i);
            recur4(dist + adj[start_index][i], i, end_index, adj);
            seen4.erase(i);
        }
    }
}

auto do_better_part2()
{
    int start_index = -1, end_index = -1;

    // 1. Scan grid for decision points
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[0].size(); j++)
        {
            if (grid[i][j] == '#')
                continue;

            if (i == 0)
            {
                start_index = dps.size();
                dps.push_back({ i, j });
                continue;
            }
            if (i == grid.size() - 1)
            {
                end_index = dps.size();
                dps.push_back({ i, j });
                continue;
            }

            int num_ways = 0;
            if (grid[i - 1][j] != '#') num_ways++;
            if (grid[i + 1][j] != '#') num_ways++;
            if (grid[i][j - 1] != '#') num_ways++;
            if (grid[i][j + 1] != '#') num_ways++;

            if (num_ways > 2)
            {
                dps.push_back({ i, j });
                continue;
            }
        }

    for (auto &dp : dps)
        cout << dp.row << "," << dp.col << endl;

    // 2. From each decision point, find neighbors and cost (indexes match the dps ones)
    vector<vector<int>> adj(dps.size(), vector<int>(dps.size()));
    for (int i = 0; i < dps.size(); i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int nrow = dps[i].row + dr[j], ncol = dps[i].col + dc[j];
            if (nrow < 0 || nrow == grid.size()) continue; // don't fall off grid
            if (grid[nrow][ncol] == '#') continue; // obstacle

            auto temp = follow_tunnel(dps[i], dr[j], dc[j]);
            if (temp.second != -1)
                adj[i][temp.first - dps.begin()] = temp.second;
        }
    }

    // Do DFS on adjacency matrix, all possible paths
    seen4.insert(0); // start dp
    recur4(0, 0, dps.size() - 1, adj);

    return max_path4;
}

int main()
{
    read_grid("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_better_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 2154);
    assert(part2 == 6654);
    return 0;
}
