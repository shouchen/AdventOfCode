#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <cassert>

std::vector<std::string> grid;
std::set<std::pair<int, int>> visited;
auto init_guard_row = 0, init_guard_col = 0;

auto read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        auto pos = line.find('^');
        if (pos != std::string::npos)
        {
            init_guard_row = int(grid.size()), init_guard_col = int(pos);
            line[pos] = '.';
        }

        grid.push_back(line);
    }
}

auto do_part1()
{
    auto guard_row = init_guard_row, guard_col = init_guard_col, row_dir = -1, col_dir = 0;

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

auto do_part2()
{
    auto retval = 0;

    for (auto &v : visited)
    {
        auto obs_row = v.first, obs_col = v.second;

        if (obs_row == init_guard_row && obs_col == init_guard_col)
            continue;

        grid[obs_row][obs_col] = '#';
        if (has_loop(init_guard_row, init_guard_col, -1, 0))
            retval++;

        grid[obs_row][obs_col] = '.';
    }

    return retval;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 4967);

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 1789);

    return 0;
}
