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

    // TODO: Num elves is constant, so below shouldn't be necessary
    auto dot_count = 0;
    for (auto i = min_row; i <= max_row; i++)
        for (auto j = min_col; j <= max_col; j++)
            if (grid.find(std::make_pair(i, j)) == grid.end())
                dot_count++;

    return dot_count;
}

auto process_input(const std::string &filename)
{
    auto retval = std::make_pair(0, 0);

    std::set<RowCol> grid;

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

    auto dir_order = 0;

    for (auto round = 1; ; round++)
    {
        std::map<RowCol, std::vector<RowCol>> proposals; // maps "to" to potential "froms"

        for (auto &pos : grid)
        {
            auto row = pos.first, col = pos.second;
            auto num_neighbors = 0;

            for (auto i = -1; i <= 1; i++)
                for (auto j = -1; j <= 1; j++)
                {
                    if (i == 0 && j == 0) continue;
                    if (grid.find(std::make_pair(row + i, col + j)) != grid.end())
                        num_neighbors++;
                }

            if (num_neighbors == 0)
                continue;

            // NSWE
            if (dir_order == 0)
            {
                if (grid.find(std::make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row - 1, col)) == grid.end() &&
                    grid.find(std::make_pair(row - 1, col + 1)) == grid.end())
                {
                    proposals[std::make_pair(row - 1, col)].push_back(pos); // N
                }
                else if (grid.find(std::make_pair(row + 1, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[std::make_pair(row + 1, col)].push_back(pos); // S
                }
                else if (grid.find(std::make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col - 1)) == grid.end())
                {
                    proposals[std::make_pair(row, col - 1)].push_back(pos); // W
                }
                else if (grid.find(std::make_pair(row - 1, col + 1)) == grid.end() &&
                    grid.find(std::make_pair(row, col + 1)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[std::make_pair(row, col + 1)].push_back(pos); // E
                }
            }
            else if (dir_order == 1) // SWEN
            {
                if (grid.find(std::make_pair(row + 1, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[std::make_pair(row + 1, col)].push_back(pos); // S
                }
                else if (grid.find(std::make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col - 1)) == grid.end())
                {
                    proposals[std::make_pair(row, col - 1)].push_back(pos); // W
                }
                else if (grid.find(std::make_pair(row - 1, col + 1)) == grid.end() &&
                    grid.find(std::make_pair(row, col + 1)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[std::make_pair(row, col + 1)].push_back(pos); // E
                }
                else if (grid.find(std::make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row - 1, col)) == grid.end() &&
                    grid.find(std::make_pair(row - 1, col + 1)) == grid.end())
                {
                    proposals[std::make_pair(row - 1, col)].push_back(pos); // N
                }
            }
            else if (dir_order == 2) // WENS
            {
                if (grid.find(std::make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col - 1)) == grid.end())
                {
                    proposals[std::make_pair(row, col - 1)].push_back(pos); // W
                }
                else if (grid.find(std::make_pair(row - 1, col + 1)) == grid.end() &&
                    grid.find(std::make_pair(row, col + 1)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[std::make_pair(row, col + 1)].push_back(pos); // E
                }
                else if (grid.find(std::make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row - 1, col)) == grid.end() &&
                    grid.find(std::make_pair(row - 1, col + 1)) == grid.end())
                {
                    proposals[std::make_pair(row - 1, col)].push_back(pos); // N
                }
                else if (grid.find(std::make_pair(row + 1, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[std::make_pair(row + 1, col)].push_back(pos); // S
                }
            }
            else if (dir_order == 3) // ENSW
            {
                if (grid.find(std::make_pair(row - 1, col + 1)) == grid.end() &&
                    grid.find(std::make_pair(row, col + 1)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[std::make_pair(row, col + 1)].push_back(pos); // E
                }
                else if (grid.find(std::make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row - 1, col)) == grid.end() &&
                    grid.find(std::make_pair(row - 1, col + 1)) == grid.end())
                {
                    proposals[std::make_pair(row - 1, col)].push_back(pos); // N
                }
                else if (grid.find(std::make_pair(row + 1, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[std::make_pair(row + 1, col)].push_back(pos); // S
                }
                else if (grid.find(std::make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row, col - 1)) == grid.end() &&
                    grid.find(std::make_pair(row + 1, col - 1)) == grid.end())
                {
                    proposals[std::make_pair(row, col - 1)].push_back(pos); // W
                }
            }
        }

        // part 2 of round
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

        if (++dir_order > 3) dir_order = 0;

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
