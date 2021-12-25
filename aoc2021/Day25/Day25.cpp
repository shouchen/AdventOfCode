#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

std::vector<std::string> grid;

auto do_step()
{
    auto moved = false;
    auto num_rows = int(grid.size()), num_cols = int(grid[0].size());
    auto next = grid;

    for (auto row = 0; row < num_rows; row++)
        for (auto col = 0; col < num_cols; col++)
        {
            auto next_col = (col + 1) % num_cols;
            if (grid[row][col] == '>' && grid[row][next_col] == '.')
            {
                next[row][next_col] = '>';
                next[row][col] = '.';
                col++;
                moved = true;
            }
            else
                next[row][col] = grid[row][col];
        }

    grid = next;

    for (auto col = 0; col < num_cols; col++)
        for (auto row = 0; row < num_rows; row++)
        {
            auto next_row = (row + 1) % num_rows;
            if (grid[row][col] == 'v' && grid[next_row][col] == '.')
            {
                next[next_row][col] = 'v';
                next[row][col] = '.';
                row++;
                moved = true;
            }
            else
                next[row][col] = grid[row][col];
        }

    grid = next;
    return moved;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
        grid.push_back(line);

    auto move = 1;

    while (do_step())
        move++;

    return move;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: N/A" << std::endl;

    assert(part1 == 419);
    return 0;
}
