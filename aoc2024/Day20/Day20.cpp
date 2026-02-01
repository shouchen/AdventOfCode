#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>
#include <cassert>

struct Location { int row = 0, col = 0; };
struct Direction { int row = 0, col = 0; };

std::vector<std::vector<int>> min_cost;

auto get_shortest_path(std::vector<std::string> &grid, Location start, Location end)
{
    static const Direction dirs[] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

    auto contains_path = std::vector<std::vector<bool>>(grid.size(), std::vector<bool>(grid[0].size(), false));
    min_cost = std::vector<std::vector<int>>(grid.size(), std::vector<int>(grid[0].size(), INT_MAX));

    std::queue<Location> q;
    q.push(start);
    min_cost[start.row][start.col] = 0;

    while (!q.empty())
    {
        Location curr = q.front(); q.pop();
        auto cost = min_cost[curr.row][curr.col];

        for (auto &d : dirs)
        {
            auto next_row = curr.row + d.row, next_col = curr.col + d.col;
            if (grid[next_row][next_col] != '#' && min_cost[next_row][next_col] == INT_MAX)
            {
                min_cost[next_row][next_col] = cost + 1;
                q.push({ next_row, next_col });
            }
        }
    }

    std::vector<Location> path;
    auto curr = end;

    path.push_back(curr);
    contains_path[curr.row][curr.col] = true;

    while (curr.row != start.row || curr.col != start.col)
        for (auto &d : dirs)
        {
            auto next_row = curr.row + d.row, next_col = curr.col + d.col;

            if (min_cost[curr.row + d.row][curr.col + d.col] == min_cost[curr.row][curr.col] - 1)
            {
                curr = { next_row, next_col };
                path.push_back(curr);
                contains_path[next_row][next_col] = true;
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
    std::vector<std::string> grid;
    Location start, end;

    while (file >> line)
    {
        auto s = line.find('S'), e = line.find('E');
        if (s != std::string::npos) start = { int(grid.size()), int(s) }, line[s] = '.';
        if (e != std::string::npos) end = { int(grid.size()), int(e) }, line[e] = '.';

        grid.push_back(line);
    }

    auto path = get_shortest_path(grid, start, end);
    auto retval = std::make_pair(0, 0);

    for (auto p1 = path.cbegin(); p1 != path.cend(); p1++)
        for (auto p2 = p1 + 1; p2 != path.cend(); p2++)
        {
            auto manhattan_dist = std::abs(p2->row - p1->row) + std::abs(p2->col - p1->col);
            if (manhattan_dist > 20)
                continue;

            auto path_dist = min_cost[p2->row][p2->col] - min_cost[p1->row][p1->col];
            if (path_dist - manhattan_dist >= 100)
            {
                if (manhattan_dist <= 2)
                    retval.first++;
                retval.second++;
            }
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
