#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>
#include <cassert>

using Grid = std::vector<std::string>;
using DistanceMap = std::vector<std::vector<int>>;

struct Location { int row = 0, col = 0; };
struct Direction { int row = 0, col = 0; };

static const Direction dirs[] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

auto compute_distances(const Grid &grid, const Location &start, const Location &end)
{
    DistanceMap dists(grid.size(), std::vector<int>(grid[0].size(), INT_MAX));

    std::queue<Location> q;
    q.push(start);
    dists[start.row][start.col] = 0;

    while (!q.empty())
    {
        Location curr = q.front(); q.pop();
        
        for (auto &d : dirs)
        {
            auto next_row = curr.row + d.row, next_col = curr.col + d.col;
            if (grid[next_row][next_col] != '#' && dists[next_row][next_col] == INT_MAX)
            {
                dists[next_row][next_col] = dists[curr.row][curr.col] + 1;
                q.push({ next_row, next_col });
            }
        }
    }

    return dists;
}

auto find_shortest_path(const DistanceMap &dists, const Location &start, const Location &end)
{
    std::vector<Location> path;
    auto curr = end;

    path.push_back(curr);

    while (curr.row != start.row || curr.col != start.col)
        for (auto &d : dirs)
        {
            auto next_row = curr.row + d.row, next_col = curr.col + d.col;

            if (dists[next_row][next_col] == dists[curr.row][curr.col] - 1)
            {
                curr = { next_row, next_col };
                path.push_back(curr);
                break;
            }
        }

    std::reverse(path.begin(), path.end());
    return path;
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    Grid grid;
    Location start, end;

    while (file >> line)
    {
        auto s = line.find('S'), e = line.find('E');
        if (s != std::string::npos) start = { int(grid.size()), int(s) }, line[s] = '.';
        if (e != std::string::npos) end = { int(grid.size()), int(e) }, line[e] = '.';

        grid.push_back(line);
    }

    auto dists = compute_distances(grid, start, end);
    auto path = find_shortest_path(dists, start, end);
    auto retval = std::make_pair(0, 0);

    for (auto p1 = path.cbegin(); p1 != path.cend(); p1++)
        for (auto p2 = p1 + 1; p2 != path.cend(); p2++)
        {
            auto manhattan_dist = std::abs(p2->row - p1->row) + std::abs(p2->col - p1->col);
            if (manhattan_dist > 20)
                continue;

            auto path_dist = dists[p2->row][p2->col] - dists[p1->row][p1->col];
            if (path_dist - manhattan_dist < 100)
                continue;

            if (manhattan_dist <= 2)
                retval.first++;
            retval.second++;
        }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 1406);
    assert(answer.second == 1006101);
    return 0;
}
