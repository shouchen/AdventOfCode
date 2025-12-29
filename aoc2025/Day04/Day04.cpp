#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

std::vector<std::string> grid;

auto read_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}

auto is_roll_accessible(int row, int col)
{
    if (grid[row][col] != '@')
        return false;

    auto adjacents = 0;

    auto min_row = std::max(0, row - 1), max_row = std::min(static_cast<int>(grid.size()) - 1, row + 1);
    auto min_col = std::max(0, col - 1), max_col = std::min(static_cast<int>(grid[0].size()) - 1, col + 1);

    for (auto r = min_row; r <= max_row; r++)
        for (auto c = min_col; c <= max_col; c++)
            if ((r != row || c != col) && grid[r][c] == '@')
                adjacents++;

    return adjacents < 4;
}

auto solve(bool remove)
{
    auto any_removed = false;
    auto retval = 0;

    do
    {
        any_removed = false;

        for (auto row = 0; row < grid.size(); row++)
            for (auto col = 0; col < grid[0].size(); col++)
                if (is_roll_accessible(row, col))
                {
                    retval++;

                    if (remove)
                    {
                        grid[row][col] = '.';
                        any_removed = true;
                    }
                }
    } while (any_removed);

    return retval;
}

int main()
{
    read_data("input.txt");

    auto part1 = solve(false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = solve(true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 1464);
    assert(part2 == 8409);
    return 0;
}
