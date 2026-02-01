#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>
#include <cassert>

struct Location { int row = 0, col = 0; };
struct Direction { int row = 0, col = 0; };

std::vector<std::vector<int>> min_cost;
std::vector<std::vector<bool>> contains_path;

auto get_shortest_path(std::vector<std::string> &grid, Location start, Location end)
{
    static const Direction dirs[] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

    min_cost = std::vector<std::vector<int>>(grid.size(), std::vector<int>(grid[0].size(), INT_MAX));
    contains_path = std::vector<std::vector<bool>>(grid.size(), std::vector<bool>(grid[0].size(), false));

    struct State
    {
        Location loc;
        int cost;
    };

    std::queue<State> q;
    q.push(State{ start, 0 });

    while (!q.empty())
    {
        State state = q.front(); q.pop();
        auto new_state = state;

        if (min_cost[state.loc.row][state.loc.col] < state.cost)
            continue;
        min_cost[state.loc.row][state.loc.col] = state.cost;

        for (auto &d : dirs)
        {
            Location new_loc{ state.loc.row + d.row, state.loc.col + d.col };
            if (grid[new_loc.row][new_loc.col] != '#')
                q.push({ new_loc, state.cost + 1 });
        }
    }

    std::vector<Location> path;
    auto curr = end;
    path.push_back(curr);
    contains_path[curr.row][curr.col] = true;

    while (curr.row != start.row || curr.col != start.col)
        for (auto &d : dirs)
            if (min_cost[curr.row + d.row][curr.col + d.col] == min_cost[curr.row][curr.col] - 1)
            {
                curr = { curr.row + d.row, curr.col + d.col };
                path.push_back(curr);
                contains_path[curr.row][curr.col] = true;
                break;
            }

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

    // Note that this goes from end to start (backwards)
    auto path = get_shortest_path(grid, start, end);
    auto retval = std::make_pair(0, 0);

    for (auto p1 = path.cbegin(); p1 != path.cend(); p1++)
        for (auto p2 = p1 + 1; p2 != path.cend(); p2++)
        {
            auto manhattan_dist = std::abs(p1->row - p2->row) + std::abs(p1->col - p2->col);
            if (manhattan_dist > 20)
                continue;

            auto path_dist = min_cost[p1->row][p1->col] - min_cost[p2->row][p2->col];
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
