#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

auto is_roll_accessible(const std::vector<std::string> &grid, int row, int col)
{
    if (grid[row][col] != '@')
        return false;

    auto adjacents = 0;

    for (int r = row - 1; r <= row + 1; r++)
        for (int c = col - 1; c <= col + 1; c++)
        {
            if (r == row && c == col)
                continue;

            if (r < 0 || r >= grid.size() || c < 0 || c >= grid[0].size())
                continue;

            if (grid[r][c] == '@')
                adjacents++;
        }

    return adjacents < 4;
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<std::string> grid;
    std::string line;
    auto retval = 0;

    while (std::getline(file, line))
        grid.push_back(line);

    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[0].size(); col++)
            if (is_roll_accessible(grid, row, col))
                retval++;

    return retval;
}

auto solve2(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<std::string> grid;
    std::string line;
    auto retval = 0;

    while (std::getline(file, line))
        grid.push_back(line);

    for (;;)
    {
        bool any_removed = false;
        for (auto row = 0; row < grid.size(); row++)
            for (auto col = 0; col < grid[0].size(); col++)
                if (is_roll_accessible(grid, row, col))
                {
                    grid[row][col] = '.';
                    retval++;
                    any_removed = true;
                }

        if (!any_removed)
            break;
    }

    return retval;
}

int main()
{
    auto part1 = solve("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = solve2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 1464);
    assert(part2 == 8409 );
    return 0;
}
