#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <cassert>

std::vector<std::string> grid;

void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}

void expand_grid()
{
    auto rows = grid.size(), cols = grid[0].size();

    for (auto row = rows; row < 5 * rows; row++)
    {
        grid.push_back("");
        for (auto c : grid[row - rows])
            grid.back().push_back(c == '9' ? '1' : ++c);
    }

    for (auto &row : grid)
        for (auto col = cols; col < 5 * cols; col++)
        {
            auto c = row[col - cols];
            row.push_back(c == '9' ? '1' : ++c);
        }
}

// Pseudo-Dijkstra
auto do_path()
{
    using RowCol = std::pair<int, int>;
    using State = std::pair<int, RowCol>;

    std::map<RowCol, int> dist;
    std::set<RowCol> locked, visited;
    std::priority_queue<State, std::vector<State>, std::greater<State>> q;
    auto retval = 0;

    auto curr = RowCol{ 0, 0 };
    q.push({ 0, curr });
    dist[curr] = 0;
    locked.insert(std::make_pair(0, 0));
    visited.insert(std::make_pair(0, 0));

    while (!q.empty())
    {
        auto curr_cost = q.top().first;
        curr = q.top().second;
        q.pop();

        // discard stale ones because a lower-cost one was already processed
        if (curr_cost != dist[curr])
        {
            continue; // THIS IS NEVER GETTING HIT
        }

        locked.insert(curr);

        if (curr.first == grid.size() - 1 && curr.second == grid[0].size() - 1)
        {
            retval = dist[curr];
            break;
        }

        const std::vector<RowCol> next_indices
        {
            { curr.first - 1, curr.second }, { curr.first + 1, curr.second },
            { curr.first, curr.second - 1}, { curr.first, curr.second + 1 }
        };

        for (const auto &next : next_indices)
        {
            if (next == curr || next.first < 0 || next.first > grid.size() - 1 ||
                next.second < 0 || next.second > grid[0].size() - 1)
                continue;

            if (locked.find(next) != locked.end())
                continue;

            auto new_cost = grid[next.first][next.second] - '0';
            if (visited.find(next) != visited.end())
            {
                if (curr_cost + new_cost < dist[next])
                {
                    q.push({ curr_cost + new_cost, next }); // THIS IS NEVER GETTING HIT
                    dist[next] = curr_cost + new_cost;
                }
            }
            else
            {
                visited.insert(next);
                q.push({ curr_cost + new_cost, next });
                dist[next] = curr_cost + new_cost;
            }
        }
    }

    return retval;
}

auto process_input(const std::string &filename)
{
    read_grid(filename);
    auto part1 = do_path();

    expand_grid();
    auto part2 = do_path();

    return std::make_pair(part1, part2);
}

int main()
{
    auto result = process_input("input.txt");

    std::cout << "Part 1: " << result.first << std::endl;
    std::cout << "Part 2: " << result.second << std::endl;

    assert(result.first == 503);
    assert(result.second == 2853);
    return 0;
}
