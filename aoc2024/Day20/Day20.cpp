#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <cassert>

struct Location { int row = 0, col = 0; };

std::vector<std::string> grid;
Location start, end;

void dump(std::vector<std::vector<int>> min_cost)
{
    for (int i = 0; i < min_cost.size(); i++)
    {
        for (int j = 0; j < min_cost[i].size(); j++)
            if (min_cost[i][j] == INT_MAX)
                std::cout << "  .";
            else
                std::cout << std::setw(3) << min_cost[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<std::vector<int>> min_cost;

auto get_shortest_path()
{
    min_cost = std::vector<std::vector<int>>(grid.size(), std::vector<int>(grid[0].size(), INT_MAX));

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
    for (;;)
    {
        if (min_cost[curr.row - 1][curr.col] == min_cost[curr.row][curr.col] - 1)
        {
            curr = { curr.row - 1, curr.col };
            path.push_back(curr);
        }
        else if (min_cost[curr.row + 1][curr.col] == min_cost[curr.row][curr.col] - 1)
        {
            curr = { curr.row + 1, curr.col };
            path.push_back(curr);
        }
        else if (min_cost[curr.row][curr.col - 1] == min_cost[curr.row][curr.col] - 1)
        {
            curr = { curr.row, curr.col - 1 };
            path.push_back(curr);
        }
        else if (min_cost[curr.row][curr.col + 1] == min_cost[curr.row][curr.col] - 1)
        {
            curr = { curr.row, curr.col + 1 };
            path.push_back(curr);
        }

        if (curr.row == start.row && curr.col == start.col)
            break;
    }

//    dump(min_cost);
    return path;
}

bool contains(const std::vector<Location> &path, Location loc)
{
    for (auto &i : path)
        if (i.row == loc.row && i.col == loc.col)
            return true;

    return false;
}

auto do_part1(const std::string &filename)
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

    dump(min_cost);

    auto retval = 0;

    // for each cheat_time, look for 
    for (auto cheat_time = 0; cheat_time < path.size() - 1; cheat_time++)
    {
        std::cout << cheat_time << std::endl;
        Location cheat_loc = path[cheat_time];

        for (auto row = cheat_loc.row - 2; row <= cheat_loc.row + 2; row++)
            for (auto col = cheat_loc.col - 2; col <= cheat_loc.col + 2; col++)
            {
                if (std::abs(row - cheat_loc.row) + std::abs(col - cheat_loc.col) == 2)
                {
                    if (contains(path, { row, col }))
                    {
                        auto savings = min_cost[cheat_loc.row][cheat_loc.col] - min_cost[row][col] - 2;
                        if (savings > 0)
                        {
                            if (savings >= 100)
                                retval++;
                        }
                    }
                }
            }
    }

    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 1406);

    //std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
