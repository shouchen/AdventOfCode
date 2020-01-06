#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <cassert>

using Grid = std::vector<std::vector<char>>;
using RowCol = std::pair<int, int>;
using PortalMap = std::map<RowCol, RowCol>;

const RowCol dirs[] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

void read_input(const std::string &filename, Grid &grid, RowCol &start, RowCol &end, PortalMap &portals)
{
    std::fstream file(filename, std::fstream::in);
    std::string line;

    while (std::getline(file, line))
    {
        std::vector<char> v;
        for (auto i = 0U; i < line.size(); i++)
            v.push_back(line[i]);

        grid.push_back(v);
    }

    std::map<std::string, std::vector<RowCol>> labels;

    for (auto row = 0U; row < grid.size(); row++)
        for (auto col = 0U; col < grid[row].size(); col++)
        {
            if (!isupper(grid[row][col]))
                continue;

            for (auto &dir : dirs)
            {
                auto next_row = row + dir.first, next_col = col + dir.second;
                auto next_next_row = next_row + dir.first, next_next_col = next_col + dir.second;

                if (next_next_row < 0 || next_next_row >= grid.size() ||
                    next_next_col < 0 || next_next_col >= grid[0].size())
                    continue;

                if (isupper(grid[next_row][next_col]) && grid[next_next_row][next_next_col] == '.')
                {
                    std::string label_name;
                    label_name.push_back(grid[row][col]);
                    label_name.push_back(grid[next_row][next_col]);
                    std::sort(label_name.begin(), label_name.end());

                    labels[label_name].push_back(RowCol{ next_next_row, next_next_col });
                    break;
                }
            }
        }

    for (auto &curr : labels)
    {
        if (curr.first == "AA")
            start = curr.second.front();
        else if (curr.first == "ZZ")
            end = curr.second.front();
        else
        {
            portals[curr.second[0]] = curr.second[1];
            portals[curr.second[1]] = curr.second[0];
        }
    }
}

auto do_part(const Grid &grid, RowCol start, RowCol end, PortalMap &portals, bool for_part2)
{
    struct Info
    {
        RowCol row_col;
        int level, steps;
    };

    std::queue<Info> q;
    q.push(Info{ start, 0, 0 });

    std::set<std::tuple<int, int, int>> seen;
    seen.insert(std::make_tuple(start.first, start.second, 0));

    while (!q.empty())
    {
        auto curr = q.front(); q.pop();

        // Check for done
        if (curr.row_col == end && curr.level == 0)
            return curr.steps;

        // Expand in each direction
        for (auto &dir : dirs)
        {
            auto next_row = curr.row_col.first + dir.first, next_col = curr.row_col.second + dir.second;
            auto next_level = curr.level;
            auto next_cell = grid[next_row][next_col];

            if (seen.find(std::make_tuple(next_row, next_col, next_level)) != seen.end())
                continue;

            // If it's a plain empty space, enqueue it for later processing.
            if (next_cell == '.')
            {
                q.push(Info{ RowCol{next_row, next_col}, next_level, curr.steps + 1 });
                seen.insert(std::make_tuple(next_row, next_col, next_level));
                continue;
            }

            // Can't proceed if there's a wall in this direction.
            if (next_cell == '#')
                continue;

            // If this label is not a portal (i.e., "AA" or "ZZ"), treat like a wall.
            if (portals.find(curr.row_col) == portals.end())
                continue;

            // Follow the portal if rules allow.
            RowCol next = portals[curr.row_col];

            if (for_part2)
            {
                bool is_inner_portal =
                    curr.row_col.first > 2 && curr.row_col.second > 2 &&
                    curr.row_col.first < int(grid.size()) - 3 &&
                    curr.row_col.second < int(grid[curr.row_col.first].size()) - 3;

                next_level += is_inner_portal ? 1 : -1;
                if (next_level < 0)
                    continue;
            }

            q.push(Info{ next, next_level, curr.steps + 1 });
            seen.insert(std::make_tuple(next.first, next.second, next_level));
        }
    }

    return -1;
}

int main()
{
    Grid grid;
    RowCol start, end;
    PortalMap portals;
    read_input("input.txt", grid, start, end, portals);

    auto part1 = do_part(grid, start, end, portals, false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part(grid, start, end, portals, true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 476);
    assert(part2 == 5350);
    return 0;
}
