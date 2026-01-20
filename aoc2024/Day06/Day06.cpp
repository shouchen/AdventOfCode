#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <cassert>

std::vector<std::string> grid;
std::set<std::pair<int, int>> visited;

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename); // TODO: Just read the file once
    std::string line;
    auto guard_row = 0, guard_col = 0, row_dir = -1, col_dir = 0;

    while (std::getline(file, line))
    {
        auto pos = line.find('^');
        if (pos != std::string::npos)
        {
            guard_row = int(grid.size()), guard_col = int(pos);
            line[pos] = '.';
        }

        grid.push_back(line);
    }

    for (;;)
    {
        visited.insert({ guard_row, guard_col }); // TODO: don't redo this for turns

        auto new_row = guard_row + row_dir, new_col = guard_col + col_dir;

        if (new_row < 0 || new_row >= int(grid.size()) ||
            new_col < 0 || new_col >= int(grid[0].size()))
        {
            return visited.size();
        }

        if (grid[new_row][new_col] == '#')
        {
            auto temp = row_dir;
            row_dir = col_dir, col_dir = -temp;
        }
        else
        {
            guard_row += row_dir;
            guard_col += col_dir;
        }
    }
}

auto has_loop(int guard_row, int guard_col, int row_dir, int col_dir)
{
    std::set<std::pair<std::pair<int, int>, std::pair<int, int>>> visited2;

    // TODO: This is almost like part 1, refactor
    for (;;)
    {
        if (visited2.find({ { guard_row, guard_col }, { row_dir, col_dir} }) != visited2.end())
            return true;

        visited2.insert({ { guard_row, guard_col }, { row_dir, col_dir} }); // TODO: don't redo this for turns

        auto new_row = guard_row + row_dir, new_col = guard_col + col_dir;

        if (new_row < 0 || new_row >= int(grid.size()) ||
            new_col < 0 || new_col >= int(grid[0].size()))
        {
            return false;
        }

        if (grid[new_row][new_col] == '#')
        {
            auto temp = row_dir;
            row_dir = col_dir, col_dir = -temp;
        }
        else
        {
            guard_row += row_dir;
            guard_col += col_dir;
        }
    }
}

auto do_part2(const std::string &filename)
{
    grid.clear();

    std::ifstream file(filename);
    std::string line;
    auto guard_row = 0, guard_col = 0, row_dir = -1, col_dir = 0;

    while (std::getline(file, line))
    {
        auto pos = line.find('^');
        if (pos != std::string::npos)
        {
            guard_row = int(grid.size()), guard_col = int(pos);
            line[pos] = '.';
        }

        grid.push_back(line);
    }

    auto retval = 0;

    for (auto obs_row = 0; obs_row < int(grid.size()); obs_row++)
        for (auto obs_col = 0; obs_col < int(grid[obs_row].size()); obs_col++)
            if (grid[obs_row][obs_col] == '.' && visited.find({ obs_row, obs_col }) != visited.end())
            {
                grid[obs_row][obs_col] = '#';
                if (has_loop(guard_row, guard_col, row_dir, col_dir))
                    retval++;

                grid[obs_row][obs_col] = '.';
            }

    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 4967);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 1789);

    return 0;
}
