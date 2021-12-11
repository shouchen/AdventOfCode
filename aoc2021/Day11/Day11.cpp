#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <cassert>

std::vector<std::vector<char>> grid;

void flash(int row, int col, std::set<std::pair<int, int>> &flashed)
{
    auto row_col = std::make_pair(row, col);
    if (flashed.find(row_col) != flashed.end())
        return;

    flashed.insert(row_col);

    for (auto i = -1; i <= 1; i++)
        for (auto j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;

            auto new_row = row + i, new_col = col + j;

            if (new_row < 0 || new_row > grid.size() - 1 ||
                new_col < 0 || new_col > grid[new_row].size() - 1)
                continue;

            if (++grid[new_row][new_col] > '9')
                flash(new_row, new_col, flashed);
        }
}

auto do_step()
{
    std::set<std::pair<int, int>> flashed;

    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].size(); col++)
            grid[row][col]++;

    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].size(); col++)
            if (grid[row][col] > '9')
                flash(row, col, flashed);

    for (const auto &f : flashed)
        grid[f.first][f.second] = '0';

    return int(flashed.size());
}

auto process_file(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto retval = std::make_pair(0, 0);

    while (std::getline(file, line))
    {
        grid.push_back(std::vector<char>());
        for (int c : line)
            grid.back().push_back(c);
    }

    for (auto step = 1; step <= 100 || !retval.second; step++)
    {
        auto flashes = do_step();

        if (step <= 100)
            retval.first += flashes;

        if (flashes == 100)
            retval.second = step;
    }

    return retval;
}

int main()
{
    auto result = process_file("input.txt");

    std::cout << "Part One: " << result.first << std::endl;
    std::cout << "Part Two: " << result.second << std::endl;

    assert(result.first == 1599);
    assert(result.second == 418);
    return 0;
}
