#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

using WrapFunc = void(*)(int &, int &, int &, int &);

std::vector<std::string> grid;
std::string path;

void wrap1(int &row, int &col, int &dr, int &dc)
{
    for (;;)
    {
        if (row < 0)
            row = int(grid.size()) - 1;  // out top
        else if (row == grid.size())
            row = 0; // out bottom
        else if (col < 0)
            col = int(grid[row].size()) - 1; // out left
        else if (col == grid[row].size())
            col = 0; // out right

        if (col < grid[row].size() && grid[row][col] != ' ')
            break;

        row += 50 * dr, col += 50 * dc;
    }
}

void wrap2(int &row, int &col, int &dr, int &dc)
{
    if (row == -1 && col >= 50 && col < 100)
        row = col + 100, col = 0, dr = 0, dc = 1; // 1 out top
    else if (col == 49 && row >= 0 && row < 50)
        row = 149 - row, col = 0, dr = 0, dc = 1; // 1 out left
    else if (row == -1 && col >= 100 && col < 150)
        col -= 100, row = 199; // 2 out top
    else if (col == 150)
        row = 149 - row, col = 99, dr = 0, dc = -1; // 2 out right
    else if (row == 50 && col >= 100)
        row = col - 50, col = 99, dr = 0, dc = -1; // 2 out down
    else if (col == 49 && row >= 50 && row < 100)
        col = row - 50, row = 100, dr = 1, dc = 0; // 3 out left
    else if (col == 100 && row >= 50 && row < 100)
        col = row + 50, row = 49, dr = -1, dc = 0; // 3 out right
    else if (col == 100 && row >= 100 && row < 150)
        row = 149 - row, col = 149, dr = 0, dc = -1; // 4 out right
    else if (row == 150 && col >= 50 && col < 100)
        row = col + 100, col = 49, dr = 0, dc = -1; // 4 out bottom
    else if (row == 99 && col >= 0 && col < 50)
        row = col + 50, col = 50, dr = 0, dc = 1; // 5 out top
    else if (col == -1 && row >= 100 && row < 150)
        row = 149 - row, col = 50, dr = 0, dc = 1; // 5 out left
    else if (col == -1 && row >= 150 && row < 200)
        col = row - 100, row = 0, dr = 1, dc = 0; // 6 out left
    else if (col == 50 && row >= 150 && row < 200)
        col = row - 100, row = 149, dr = -1, dc = 0; // 6 out right
    else if (row == 200)
        col += 100, row = 0; // 6 out bottom
}

void move(int &row, int &col, int &dr, int &dc, int count, WrapFunc wrap)
{
    while (count--)
    {
        auto next_row = row + dr, next_col = col + dc, next_dr = dr, next_dc = dc;

        wrap(next_row, next_col, next_dr, next_dc);

        if (next_row < grid.size() && next_col < grid[next_row].size() && grid[next_row][next_col] == '#')
            break;

        row = next_row, col = next_col;
        dr = next_dr, dc = next_dc;
    }
}

void turn(int &dr, int &dc, char dir)
{
    if (dc == 0)
        dc = (dir == 'L') ? dr : -dr, dr = 0;
    else
        dr = (dir == 'L') ? -dc : dc, dc = 0;
}

auto do_part(WrapFunc wrap)
{
    auto row = 0, col = 0, dr = 0, dc = 1;
    while (grid[row][col] != '.')
        col += 50;

    auto c = ' ';
    auto number = 0;
    std::stringstream ss(path);

    while (ss >> c)
    {
        if (isdigit(c))
        {
            number = number * 10 + c - '0';
        }
        else
        {
            move(row, col, dr, dc, number, wrap);
            turn(dr, dc, c);

            number = 0;
        }
    }

    move(row, col, dr, dc, number, wrap);

    auto retval = 1000 * (row + 1) + 4 * (col + 1);

    if (dr == 1)
        retval += 1;
    else if (dc == -1)
        retval += 2;
    else if (dr == -1)
        retval += 3;

    return retval;
}

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (getline(file, line) && !line.empty())
        grid.push_back(line);

    file >> path;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part(wrap1);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part(wrap2);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 131052);
    assert(part2 == 4578);
    return 0;
}