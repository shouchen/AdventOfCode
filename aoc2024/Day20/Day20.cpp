#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>
#include <cassert>

struct Location { int row = 0, col = 0; };

std::vector<std::string> grid;
Location start, end;

std::vector<std::vector<int>> min_cost;
std::vector<std::vector<bool>> contains_path;

auto get_shortest_path()
{
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

        // up
        Location new_loc = { state.loc.row - 1, state.loc.col };
        if (grid[new_loc.row][new_loc.col] != '#')
            q.push({ new_loc, state.cost + 1 });

        // down
        new_loc = { state.loc.row + 1, state.loc.col };
        if (grid[new_loc.row][new_loc.col] != '#')
            q.push({ new_loc, state.cost + 1 });

        // left
        new_loc = { state.loc.row, state.loc.col - 1 };
        if (grid[new_loc.row][new_loc.col] != '#')
            q.push({ new_loc, state.cost + 1 });

        // right
        new_loc = { state.loc.row, state.loc.col + 1 };
        if (grid[new_loc.row][new_loc.col] != '#')
            q.push({ new_loc, state.cost + 1 });
    }

    std::vector<Location> path;
    auto curr = end;
    path.push_back(curr);
    contains_path[curr.row][curr.col] = true;

    while (curr.row != start.row || curr.col != start.col)
    {
        if (min_cost[curr.row - 1][curr.col] == min_cost[curr.row][curr.col] - 1)
        {
            curr = { curr.row - 1, curr.col };
            path.push_back(curr);
            contains_path[curr.row][curr.col] = true;
        }
        else if (min_cost[curr.row + 1][curr.col] == min_cost[curr.row][curr.col] - 1)
        {
            curr = { curr.row + 1, curr.col };
            path.push_back(curr);
            contains_path[curr.row][curr.col] = true;
        }
        else if (min_cost[curr.row][curr.col - 1] == min_cost[curr.row][curr.col] - 1)
        {
            curr = { curr.row, curr.col - 1 };
            path.push_back(curr);
            contains_path[curr.row][curr.col] = true;
        }
        else if (min_cost[curr.row][curr.col + 1] == min_cost[curr.row][curr.col] - 1)
        {
            curr = { curr.row, curr.col + 1 };
            path.push_back(curr);
            contains_path[curr.row][curr.col] = true;
        }
    }

    return path;
}

auto solve(const std::string &filename, int max_cheat_len)
{
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
    {
        auto s = line.find('S'), e = line.find('E');
        if (s != std::string::npos) start = { int(grid.size()), int(s) }, line[s] = '.';
        if (e != std::string::npos) end = { int(grid.size()), int(e) }, line[e] = '.';

        grid.push_back(line);
    }

    // Note that this goes from end to start (backwards)
    auto path = get_shortest_path();
    auto retval = 0;

    for (auto cheat_time = 0; cheat_time < path.size() - 1; cheat_time++)
    {
        Location cheat_loc = path[cheat_time];

        for (auto row = std::max(cheat_loc.row - max_cheat_len, 0); row <= std::min(cheat_loc.row + max_cheat_len, int(grid.size()) - 1); row++)
            for (auto col = std::max(cheat_loc.col - max_cheat_len, 0); col <= std::min(cheat_loc.col + max_cheat_len, int(grid[row].size()) - 1); col++)
            {
                auto cheat_len = std::abs(row - cheat_loc.row) + std::abs(col - cheat_loc.col);
                if (cheat_len <= max_cheat_len && contains_path[row][col] &&
                    min_cost[cheat_loc.row][cheat_loc.col] - min_cost[row][col] - cheat_len >= 100)
                {
                    retval++;
                }
            }
    }

    return retval;
}

int main()
{
    auto part1 = solve("input.txt", 2);
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 1406);

    auto part2 = solve("input.txt", 20);
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 1006101);

    return 0;
}
