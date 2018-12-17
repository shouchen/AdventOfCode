#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cassert>

char grid[1820][700];
int min_depth = std::numeric_limits<int>::max();
int max_depth = std::numeric_limits<int>::min();
int min_x = std::numeric_limits<int>::max();
int max_x = std::numeric_limits<int>::min();

void dump()
{
    auto count = 0U, count2 = 0U;
    for (auto row = 0; row <= max_depth + 2; row++)
    {
        std::cout << std::setw(4) << row << " ";
        for (auto col = min_x - 1; col <= max_x + 1; col++)
        {
            auto c = grid[row][col];
            std::cout << (c ? c : '.');
            if ((c == '|' || c == '~') && row >= min_depth) count++;
            if (c == '~' && row >= min_depth) count2++;
        }
        std::cout << " " << std::setw(5) << count << " " << count2 << std::endl;
    }
}

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    auto var1 = 'x', equals = '=', var2 = 'y', comma = ',', dot = '.';
    auto dim1 = 0, dim2a = 0, dim2b = 0;

    grid[0][500] = '+';

    while (file >> var1 >> equals >> dim1 >> comma >> var2 >> equals >> dim2a >> dot >> dot >> dim2b)
    {
        if (var1 == 'x')
        {
            for (auto row = dim2a; row <= dim2b; row++)
                grid[row][dim1] = '#';
            min_depth = std::min(min_depth, dim2a);
            max_depth = std::max(max_depth, dim2b);
            min_x = std::min(min_x, dim1);
            max_x = std::max(max_x, dim1);
        }
        else
        {
            for (auto col = dim2a; col <= dim2b; col++)
                grid[dim1][col] = '#';
            min_depth = std::min(min_depth, dim1);
            max_depth = std::max(max_depth, dim1);
            min_x = std::min(min_x, dim2a);
            max_x = std::max(max_x, dim2b);
        }
    }
}

void seep(int row, int col)
{
    if (row > max_depth || col < min_x - 1 || col > max_x + 1)
        return;

    if (grid[row][col])
        return;

    grid[row][col] = '|';

    seep(row + 1, col);

    if (grid[row + 1][col] != '#' && grid[row + 1][col] != '~')
        return;

    seep(row, col - 1);

    if (grid[row][col + 1] == '#')
    {
        auto test_col = col;
        while (grid[row][test_col] == '|') --test_col;

        if (grid[row][test_col] == '#')
            while (grid[row][col] == '|')
                grid[row][col--] = '~';
    }
    else
        seep(row, col + 1);
}

void do_parts1_and_2(unsigned &part1, unsigned &part2)
{
    seep(1, 500);

    part1 = 0, part2 = 0;
    for (auto row = min_depth; row <= max_depth; row++)
    {
        for (auto col = min_x - 1; col <= max_x + 1; col++)
        {
            auto c = grid[row][col];
            if (c == '|' || c == '~') part1++;
            if (c == '~') part2++;
        }
    }
}

int main()
{
    read_input("input.txt");

    auto part1 = 0U, part2 = 0U;
    do_parts1_and_2(part1, part2);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 31953);
    assert(part2 == 26410);

    return 0;
}
