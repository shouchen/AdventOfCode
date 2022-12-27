#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

std::vector<std::string> grid;

void move(int &row, int &col, int dr, int dc, int count)
{
    auto next_row = row, next_col = col;

    for (auto i = 0; i < count; i++)
    {
        next_row += dr;
        next_col += dc;

        if (next_row < 0)
            next_row = int(grid.size()) - 1;
        else if (next_row == grid.size())
            next_row = 0;

        if (next_col < 0)
            next_col = int(grid[0].size()) - 1;
        else if (next_col == grid[0].size())
            next_col = 0;

        switch (grid[next_row][next_col])
        {
        case '#':
            return;
        case '.':
            row = next_row;
            col = next_col;
            break;
        case ' ':
            count++;
            break;
        }
    }
}

void turn(int &dr, int &dc, char dir)
{
    if (dr == -1 && dc == 0)
    {
        if (dir == 'L') dr = 0, dc = -1;
        else dr = 0, dc = 1;
    }
    else if (dr == 1 && dc == 0)
    {
        if (dir == 'L') dr = 0, dc = 1;
        else dr = 0, dc = -1;
    }
    else if (dr == 0 && dc == -1)
    {
        if (dir == 'L') dr = 1, dc = 0;
        else dr = -1, dc = 0;
    }
    else if (dr == 0 && dc == 1)
    {
        if (dir == 'L') dr = -1, dc = 0;
        else dr = 1, dc = 0;
    }
}

auto do_part1(const std::string &filename)
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

    std::string number;
    auto c = ' ';

    while (file >> c)
    {
        if (isdigit(c))
        {
            number.push_back(c);
        }
        else
        {
            move(curr_row, curr_col, dr, dc, atoi(number.c_str()));
            turn(dr, dc, c);

            number.clear();
        }
    }

    if (!number.empty())
        move(curr_row, curr_col, dr, dc, atoi(number.c_str()));

    auto retval = 1000 * (curr_row + 1) + 4 * (curr_col + 1);
    if (dr == -1 && dc == 0)
        retval += 3;
    else if (dr == 1 && dc == 0)
        retval += 1;
    else if (dr == 0 && dc == -1)
        retval += 2;
    else if (dr ==0 && dc == 1)
        retval += 0;
    return retval;
}

void move2(int &row, int &col, int &dr, int &dc, int count)
{
    auto next_row = row, next_col = col, next_dr = dr, next_dc = dc;

    for (int i = 0; i < count; i++)
    {
        next_row += dr, next_col += dc;

        // special wrapping
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
            return;

        row = next_row, col = next_col;
        dr = next_dr, dc = next_dc;
    }
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, number;

    while (getline(file, line) && !line.empty())
        grid.push_back(line);

    auto curr_row = 0, curr_col = 0, dr = 0, dc = 1;
    while (grid[curr_row][curr_col] != '.')
        curr_col++;

    auto c = ' ';

    while (file >> c)
    {
        if (isdigit(c))
        {
            number.push_back(c);
        }
        else
        {
            move2(curr_row, curr_col, dr, dc, atoi(number.c_str()));
            turn(dr, dc, c);

            number.clear();
        }
    }

    if (!number.empty())
        move2(curr_row, curr_col, dr, dc, atoi(number.c_str()));

    auto retval = 1000 * (curr_row + 1) + 4 * (curr_col + 1);
    if (dr == -1 && dc == 0)
        retval += 3;
    else if (dr == 1 && dc == 0)
        retval += 1;
    else if (dr == 0 && dc == -1)
        retval += 2;
    else if (dr == 0 && dc == 1)
        retval += 0;
    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 131052);
    assert(part2 == 4578);
    return 0;
}