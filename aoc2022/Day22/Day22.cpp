#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

using MoveFunc = void(*)(int &, int &, int &, int &, int);

std::vector<std::string> grid;

void move1(int &row, int &col, int &dr, int &dc, int count)
{
    auto next_row = row, next_col = col, next_dr = dr, next_dc = dc;

    while (count--)
    {
        next_row += dr, next_col += dc;

        // part1 wrapping rules
        if (next_row < 0)
            next_row = int(grid.size()) - 1;
        else if (next_row == grid.size())
            next_row = 0;

        if (next_col < 0)
            next_col = int(grid[0].size()) - 1;
        else if (next_col == grid[0].size())
            next_col = 0;

        if (grid[next_row][next_col] == ' ')
        {
            count++;
            continue;
        }

        if (grid[next_row][next_col] == '#')
            break;

        row = next_row, col = next_col;
        dr = next_dr, dc = next_dc;
    }
}

void move2(int &row, int &col, int &dr, int &dc, int count)
{
    auto next_row = row, next_col = col, next_dr = dr, next_dc = dc;

    while (count--)
    {
        next_row += dr, next_col += dc;

        // part2 wrapping rules
        if (next_row == -1 && next_col >= 50 && next_col < 100)
            next_row = next_col + 100, next_col = 0, next_dr = 0, next_dc = 1; // 1 out top
        else if (next_col == 49 && next_row >= 0 && next_row < 50)
            next_row = 149 - next_row, next_col = 0, next_dr = 0, next_dc = 1; // 1 out left
        else if (next_row == -1 && next_col >= 100 && next_col < 150)
            next_col -= 100, next_row = 199; // 2 out top
        else if (next_col == 150)
            next_row = 149 - next_row, next_col = 99, next_dr = 0, next_dc = -1; // 2 out right
        else if (next_row == 50 && next_col >= 100)
            next_row = next_col - 50, next_col = 99, next_dr = 0, next_dc = -1; // 2 out down
        else if (next_col == 49 && next_row >= 50 && next_row < 100)
            next_col = next_row - 50, next_row = 100, next_dr = 1, next_dc = 0; // 3 out left
        else if (next_col == 100 && next_row >= 50 && next_row < 100)
            next_col = next_row + 50, next_row = 49, next_dr = -1, next_dc = 0; // 3 out right
        else if (next_col == 100 && next_row >= 100 && next_row < 150)
            next_row = 149 - next_row, next_col = 149, next_dr = 0, next_dc = -1; // 4 out right
        else if (next_row == 150 && next_col >= 50 && next_col < 100)
            next_row = next_col + 100, next_col = 49, next_dr = 0, next_dc = -1; // 4 out bottom
        else if (next_row == 99 && next_col >= 0 && next_col < 50)
            next_row = next_col + 50, next_col = 50, next_dr = 0, next_dc = 1; // 5 out top
        else if (next_col == -1 && next_row >= 100 && next_row < 150)
            next_row = 149 - next_row, next_col = 50, next_dr = 0, next_dc = 1; // 5 out left
        else if (next_col == -1 && next_row >= 150 && next_row < 200)
            next_col = next_row - 100, next_row = 0, next_dr = 1, next_dc = 0; // 6 out left
        else if (next_col == 50 && next_row >= 150 && next_row < 200)
            next_col = next_row - 100, next_row = 149, next_dr = -1, next_dc = 0; // 6 out right
        else if (next_row == 200)
            next_col += 100, next_row = 0; // 6 out bottom

        if (grid[next_row][next_col] == '#')
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

auto compute_passcode(int curr_row, int curr_col, int dr, int dc)
{
    auto retval = 1000 * (curr_row + 1) + 4 * (curr_col + 1);

    if (dr == -1 && dc == 0)
        retval += 3;
    else if (dr == 1 && dc == 0)
        retval += 1;
    else if (dr == 0 && dc == -1)
        retval += 2;

    return retval;
}

auto do_part(const std::string &filename, MoveFunc move)
{
    std::ifstream file(filename);
    std::string line;
    auto max_line = 0;

    while (getline(file, line) && !line.empty())
    {
        grid.push_back(line);
        if (line.length() > max_line)
            max_line = int(line.length());
    }

    for (auto &i : grid)
        while (i.length() < max_line)
            i.push_back(' ');

    auto curr_row = 0, curr_col = 0, dr = 0, dc = 1;
    while (grid[curr_row][curr_col] != '.')
        curr_col++;

    auto c = ' ';
    auto number = 0;

    while (file >> c)
    {
        if (isdigit(c))
        {
            number = number * 10 + c - '0';
        }
        else
        {
            move(curr_row, curr_col, dr, dc, number);
            turn(dr, dc, c);

            number = 0;
        }
    }

    move(curr_row, curr_col, dr, dc, number);

    return compute_passcode(curr_row, curr_col, dr, dc);
}

int main()
{
    auto part1 = do_part("input.txt", move1);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part("input.txt", move2);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 131052);
    assert(part2 == 4578);
    return 0;
}