#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

std::vector<std::string> grid;

auto robot_row = 0, robot_col = 0;

auto dump()
{
    for (auto &row : grid)
        std::cout << row << std::endl;
    std::cout << std::endl;
}

auto get_total_for_config()
{
    auto retval = 0ULL;
    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].size(); col++)
            if (grid[row][col] == 'O' || grid[row][col] == '[')
                retval += row * 100 + col;
    return retval;
}

auto do_move(char move)
{
    auto d_row = 0, d_col = 0;

    if (move == '^') d_row = -1;
    else if (move == 'v') d_row = 1;
    else if (move == '<') d_col = -1;
    else if (move == '>') d_col = 1;

    // Find next empty space or wall in direction of travel.
    auto row = robot_row + d_row, col = robot_col + d_col;
    while (grid[row][col] != '#' && grid[row][col] != '.')
        row += d_row, col += d_col;

    // Find next space in direction of travel.
    auto next_row = robot_row + d_row, next_col = robot_col + d_col;

    // Blocked?
    if (grid[row][col] == '#')
        return false;

    // Simple Move?
    if (grid[next_row][next_col] == '.')
    {
        grid[robot_row][robot_col] = '.';
        grid[robot_row + d_row][robot_col + d_col] = '@';
        robot_row = next_row, robot_col = next_col;
        return true;
    }

    // Else, it's a push
    grid[robot_row][robot_col] = '.';
    grid[robot_row + d_row][robot_col + d_col] = '@';
    robot_row = next_row, robot_col = next_col;
    grid[row][col] = 'O';
    return  true;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line) && line.length())
    {
        auto at_pos = line.find('@');
        if (at_pos != std::string::npos)
            robot_row = int(grid.size()), robot_col = int(at_pos);
        grid.push_back(line);
    }

    //std::cout << "Initial State:" << std::endl;
    //dump();

    auto move = ' ';
    while (file >> move)
    {
        do_move(move);
        //std::cout << "Move " << move << ":" << std::endl;
        //dump();
    }

    return get_total_for_config();
}

// See if it will be possible to move the object at (row, col) in the specified direction
// without actually performing the move.
auto can_move2(int row, int col, char move)
{
    if (grid[row][col] == '.')
        return true;

    if (grid[row][col] == '#')
        return false;

    // Align with start of object, like all other cases
    if (grid[row][col] == ']')
        col--;

    auto d_row = 0, d_col = 0;

    if (move == '^') d_row = -1;
    else if (move == 'v') d_row = 1;
    else if (move == '<') d_col = -1;
    else if (move == '>') d_col = 1;

    auto next_row = row + d_row, next_col = col + d_col;
    if (grid[row][col] == '[' && move == '>') next_col++;
    return
        can_move2(next_row, next_col, move) &&
        (grid[row][col] != '[' || (move != '^' && move != 'v') || can_move2(next_row, next_col + 1, move));
}

// Move the object at the location in the direction indicated. Should first call can_move2() 
// to see whether the way is open or not. Otherwise, this function would have to back out
// partial changes. TODO: Make this one function with a "test" bool parameter since the
// traversal actions are essentially the same.
auto move2(int row, int col, char move)
{
    if (grid[row][col] == '.')
        return true;

    if (grid[row][col] == '#')
        return false;

    // Align with start of a double-wide object, to match all other cases
    if (grid[row][col] == ']')
        col--;

    auto d_row = 0, d_col = 0;

    if (move == '^') d_row = -1;
    else if (move == 'v') d_row = 1;
    else if (move == '<') d_col = -1;
    else if (move == '>') d_col = 1;

    auto next_row = row + d_row, next_col = col + d_col;
    if (grid[row][col] == '[' && move == '>') next_col++;

    if (move2(next_row, next_col, move) &&
        (grid[row][col] != '[' || (move != '^' && move != 'v') || move2(next_row, next_col + 1, move)))
    {
        if (grid[row][col] == '[' && move == '>') --next_col;

        // Special case for double-wide moves
        if (grid[row][col] == '[')
        {
            if (d_row == 0) // horizontal moves
            {
                grid[next_row][next_col] = '[';
                grid[next_row][next_col + 1] = ']';
                if (move == '>')
                {
                    grid[row][col] = '.';
                }
                else
                    grid[row - d_row][col - d_col] = '.';
            }
            else // vertical moves
            {
                grid[next_row][next_col] = '[';
                grid[next_row][next_col + 1] = ']';
                grid[row][col] = '.';
                grid[row][col + 1] = '.';
            }
        }
        else // single-wide moves
        {
            if (grid[row][col] == '@') robot_row = next_row, robot_col = next_col;
            grid[next_row][next_col] = grid[row][col];
            grid[row][col] = '.';

        }
    }

    return true;
}

auto do_part2(const std::string &filename)
{
    grid.clear();

    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line) && line.length())
    {
        std::string line2;
        for (auto c : line)
            if (c == '@')
                line2.push_back(c), line2.push_back('.');
            else if (c == 'O')
                line2.push_back('['), line2.push_back(']');
            else
                line2.push_back(c), line2.push_back(c);

        auto at_pos = line2.find('@');
        if (at_pos != std::string::npos)
            robot_row = int(grid.size()), robot_col = int(at_pos);
        grid.push_back(line2);
    }

    std::cout << "Initial State:" << std::endl;
    dump();

    auto move = ' ';
    while (file >> move)
    {
        //std::cout << "Move " << move << ":" << std::endl;
        if (can_move2(robot_row, robot_col, move))
            move2(robot_row, robot_col, move);
        //dump();
    }

    return get_total_for_config();
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 1438161);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 1437981);

    return 0;
}
