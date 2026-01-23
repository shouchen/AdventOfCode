#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

std::vector<std::string> grid;

auto read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}

auto spells_xmas(int row, int col, int row_dir, int col_dir)
{
    static std::string xmas = "XMAS";

    for (auto c : xmas)
    {
        if (row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size())
            return false;

        if (grid[row][col] != c)
            return false;

        row += row_dir, col += col_dir;
    }

    return true;
}

auto has_x_mases(int row, int col)
{
    if (grid[row][col] != 'A')
        return false;

    if ((grid[row - 1][col - 1] != 'M' || grid[row + 1][col + 1] != 'S') &&
        (grid[row - 1][col - 1] != 'S' || grid[row + 1][col + 1] != 'M'))
        return false;

    if ((grid[row - 1][col + 1] != 'M' || grid[row + 1][col - 1] != 'S') &&
        (grid[row - 1][col + 1] != 'S' || grid[row + 1][col - 1] != 'M'))
        return false;

    return true;
}

auto do_part1()
{
    auto retval = 0;

    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].size(); col++)
            for (auto row_dir = -1; row_dir <= 1; row_dir++)
                for (auto col_dir = -1; col_dir <= 1; col_dir++)
                    if (spells_xmas(row, col, row_dir, col_dir))
                        retval++;

    return retval;
}

auto do_part2()
{
    auto retval = 0;

    for (auto row = 1; row < grid.size() - 1; row++)
        for (auto col = 1; col < grid[row].size() - 1; col++)
            retval += has_x_mases(row, col);

    return retval;
}

int main()
{
    read_grid("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 2500);

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 1933);

    return 0;
}
