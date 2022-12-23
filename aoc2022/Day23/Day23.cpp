#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <cassert>

using RowCol = std::pair<int, int>;

auto get_dot_count(const std::set<RowCol> &grid)
{
    auto min_row = INT_MAX, max_row = INT_MIN, min_col = INT_MAX, max_col = INT_MIN;
    for (auto &i : grid)
    {
        min_row = std::min(i.first, min_row);
        max_row = std::max(i.first, max_row);
        min_col = std::min(i.second, min_col);
        max_col = std::max(i.second, max_col);
    }

    return (max_row - min_row + 1) * (max_col - min_col + 1) - grid.size();
}

auto check_north_proposal(const std::set<RowCol> &grid, int row, int col, std::map<RowCol, std::vector<RowCol>> &proposals)
{
    if (grid.find(std::make_pair(row - 1, col - 1)) == grid.end() &&
        grid.find(std::make_pair(row - 1, col)) == grid.end() &&
        grid.find(std::make_pair(row - 1, col + 1)) == grid.end())
    {
        proposals[std::make_pair(row - 1, col)].push_back(std::make_pair(row, col));
        return true;
    }

    return false;
}

auto check_south_proposal(const std::set<RowCol> &grid, int row, int col, std::map<RowCol, std::vector<RowCol>> &proposals)
{
    if (grid.find(std::make_pair(row + 1, col - 1)) == grid.end() &&
        grid.find(std::make_pair(row + 1, col)) == grid.end() &&
        grid.find(std::make_pair(row + 1, col + 1)) == grid.end())
    {
        proposals[std::make_pair(row + 1, col)].push_back(std::make_pair(row, col));
        return true;
    }

    return false;
}

auto check_west_proposal(const std::set<RowCol> &grid, int row, int col, std::map<RowCol, std::vector<RowCol>> &proposals)
{
    if (grid.find(std::make_pair(row - 1, col - 1)) == grid.end() &&
        grid.find(std::make_pair(row, col - 1)) == grid.end() &&
        grid.find(std::make_pair(row + 1, col - 1)) == grid.end())
    {
        proposals[std::make_pair(row, col - 1)].push_back(std::make_pair(row, col));
        return true;
    }

    return false;
}

auto check_east_proposal(const std::set<RowCol> &grid, int row, int col, std::map<RowCol, std::vector<RowCol>> &proposals)
{
    if (grid.find(std::make_pair(row - 1, col + 1)) == grid.end() &&
        grid.find(std::make_pair(row, col + 1)) == grid.end() &&
        grid.find(std::make_pair(row + 1, col + 1)) == grid.end())
    {
        proposals[std::make_pair(row, col + 1)].push_back(std::make_pair(row, col));
        return true;
    }

    return false;
}

auto count_neighbors(const std::set<RowCol> &grid, int row, int col)
{
    auto num_neighbors = 0;

    for (auto i = -1; i <= 1; i++)
        for (auto j = -1; j <= 1; j++)
            if ((i != 0 || j != 0) && grid.find(std::make_pair(row + i, col + j)) != grid.end())
                num_neighbors++;

    return num_neighbors;
}

auto process_input(const std::string &filename)
{
    std::set<RowCol> grid;
    auto retval = std::make_pair(0, 0);

    std::ifstream file(filename);
    std::string line;
    auto row = 0;

    while (getline(file, line))
    {
        for (auto col = 0; col < line.length(); col++)
            if (line[col] == '#')
                grid.insert(std::make_pair(row, col));
        row++;
    }

    for (auto round = 1; ; round++)
    {
        std::map<RowCol, std::vector<RowCol>> proposals; // maps "to" to potential "froms"

        for (auto &pos : grid)
        {
            auto row = pos.first, col = pos.second;

            if (count_neighbors(grid, row, col) == 0)
                continue;

            if (round % 4 == 1)
            {
                check_north_proposal(grid, row, col, proposals) ||
                check_south_proposal(grid, row, col, proposals) ||
                check_west_proposal(grid, row, col, proposals) ||
                check_east_proposal(grid, row, col, proposals);
            }
            else if (round % 4 == 2)
            {
                check_south_proposal(grid, row, col, proposals) ||
                check_west_proposal(grid, row, col, proposals) ||
                check_east_proposal(grid, row, col, proposals) ||
                check_north_proposal(grid, row, col, proposals);
            }
            else if (round % 4 == 3)
            {
                check_west_proposal(grid, row, col, proposals) ||
                check_east_proposal(grid, row, col, proposals) ||
                check_north_proposal(grid, row, col, proposals) ||
                check_south_proposal(grid, row, col, proposals);
            }
            else if (round % 4 == 0)
            {
                check_east_proposal(grid, row, col, proposals) ||
                check_north_proposal(grid, row, col, proposals) ||
                check_south_proposal(grid, row, col, proposals) ||
                check_west_proposal(grid, row, col, proposals);
            }
        }

        // second half of round
        auto elf_moved = false;
        for (auto &p : proposals)
        {
            if (p.second.size() == 1)
            {
                grid.insert(p.first);
                grid.erase(grid.find(p.second[0]));
                elf_moved = true;
            }
        }

        if (!elf_moved)
        {
            retval.second = round;
            return retval;
        }

        if (round == 10)
            retval.first = get_dot_count(grid);
    }
}

int main()
{
    auto output = process_input("input.txt");
    std::cout << "Part One: " << output.first << std::endl;
    std::cout << "Part Two: " << output.second << std::endl;

    assert(output.first == 4254);
    assert(output.second == 992);
    return 0;
}
