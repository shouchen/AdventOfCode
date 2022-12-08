#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

std::vector<std::string> grid;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
        grid.push_back(line);
}

auto all_trees_below_height(int row1, int col1, int row2, int col2, int height)
{
    for (auto row = row1; row <= row2; row++)
        for (int col = col1; col <= col2; col++)
            if (grid[row][col] >= height)
                return false;

    return true;
}

auto is_visible(int row, int col)
{
    return
        all_trees_below_height(0, col, row - 1, col, grid[row][col]) ||
        all_trees_below_height(row + 1, col, grid.size() - 1, col, grid[row][col]) ||
        all_trees_below_height(row, 0, row, col - 1, grid[row][col]) ||
        all_trees_below_height(row, col + 1, row, grid[row].size() - 1, grid[row][col]);
}

auto scenic_score(int row, int col)
{
    auto up = 0, down = 0, left = 0, right = 0;

    for (auto r = row - 1; r >= 0; --r)
    {
        up++;
        if (grid[r][col] >= grid[row][col])
            break;
    }

    for (auto r = row + 1; r < grid.size(); r++)
    {
        down++;
        if (grid[r][col] >= grid[row][col])
            break;
    }

    for (auto c = col - 1; c >= 0; --c)
    {
        left++;
        if (grid[row][c] >= grid[row][col])
            break;
    }

    for (auto c = col + 1; c < grid[0].size(); c++)
    {
        right++;
        if (grid[row][c] >= grid[row][col])
            break;
    }

    return up * down * left * right;
}

auto do_part1()
{
    auto retval = 0;

    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].length() ; col++)
            if (is_visible(row, col))
                retval++;

    return retval;
}

auto do_part2()
{
    auto retval = 0;

    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[0].length(); col++)
            retval = std::max(scenic_score(row, col), retval);

    return retval;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 1787);
    assert(part2 == 440640);
    return 0;
}
