#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <set>
#include <bitset>
#include <cassert>

using RowCol = std::pair<int, int>;

const RowCol dirs[] { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } };

std::vector<std::vector<char>> grid;
RowCol origin;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<char> v;
    std::string line;

    grid.clear();

    while (std::getline(file, line))
    {
        for (auto i = 0U; i < line.size(); i++)
        {
            v.push_back(line[i]);
            if (v[i] == '@')
                origin = std::make_pair(grid.size(), i);
        }

        grid.push_back(v);
        v.clear();
    }
}

auto bfs(RowCol start, unsigned keyset)
{
    struct Info
    {
        int row, col;
        unsigned keyset, steps;
    };

    std::set<std::tuple<int, int, int>> seen; // by row/col/keyset
    std::queue<Info> q;

    q.push(Info{ start.first, start.second, keyset, 0 });

    while (!q.empty())
    {
        auto curr = q.front(); q.pop();
        std::bitset<26> visited = curr.keyset;

        if (visited.all())
            return curr.steps;

        for (auto &dir : dirs)
        {
            auto next_row = curr.row + dir.first, next_col = curr.col + dir.second;
            visited = curr.keyset;

            auto next_cell = grid[next_row][next_col];
            if (next_cell == '#')
                continue;

            if (seen.find(std::make_tuple(next_row, next_col, curr.keyset)) != seen.end())
                continue;

            if (isupper(next_cell) && !visited[next_cell - 'A'])
                continue;

            if (islower(next_cell))
                visited[next_cell - 'a'] = true;

            q.push(Info{ next_row, next_col, visited.to_ulong(), curr.steps + 1 });
            seen.insert(std::make_tuple(next_row, next_col, visited.to_ulong()));
        }
    }

    return 0U;
}

auto do_part1(const std::string &filename)
{
    read_input(filename);
    return bfs(origin, 0);
}

auto do_part2(const std::string &filename)
{
    read_input(filename);

    // Modify input for part 2
    auto origin1 = std::make_pair(origin.first - 1, origin.second - 1);
    auto origin2 = std::make_pair(origin.first - 1, origin.second + 1);
    auto origin3 = std::make_pair(origin.first + 1, origin.second - 1);
    auto origin4 = std::make_pair(origin.first + 1, origin.second + 1);

    grid[origin.first - 1][origin.second] = '#';
    grid[origin.first][origin.second - 1] = '#';
    grid[origin.first][origin.second + 1] = '#';
    grid[origin.first + 1][origin.second] = '#';

    // If a key isn't in a quadrant, mark it already obtained for that quadrant.
    std::bitset<26> q1_keyset, q2_keyset, q3_keyset, q4_keyset;
    for (auto row = 0; row < int(grid.size()); row++)
        for (auto col = 0; col < int(grid[0].size()); col++)
        {
            auto cell = grid[row][col];
            if (!islower(cell))
                continue;

            auto key_index = cell - 'a';
            q1_keyset[key_index] = row > origin1.first || col > origin1.second;
            q2_keyset[key_index] = row > origin2.first || col < origin2.second;
            q3_keyset[key_index] = row < origin3.first || col > origin3.second;
            q4_keyset[key_index] = row < origin4.first || col < origin4.second;
        }

    return
        bfs(origin1, q1_keyset.to_ulong()) +
        bfs(origin2, q2_keyset.to_ulong()) +
        bfs(origin3, q3_keyset.to_ulong()) +
        bfs(origin4, q4_keyset.to_ulong());
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "part one: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 4844);
    assert(part2 == 1784);
    return 0;
}
