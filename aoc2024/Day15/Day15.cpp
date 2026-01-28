#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

struct Location { int row = 0, col = 0; };
struct Direction { int row, col; Direction(char move); };

std::vector<std::string> grid;
Location robot;

Direction::Direction(char move)
{
    switch (move)
    {
    case '^': row = -1, col =  0; break;
    case 'v': row =  1, col =  0; break;
    case '<': row =  0, col = -1; break;
    case '>': row =  0, col =  1; break;
    }
}

auto get_total_for_config()
{
    auto retval = 0;
    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].size(); col++)
            if (grid[row][col] == 'O' || grid[row][col] == '[')
                retval += row * 100 + col;
    return retval;
}

auto do_move(Direction dir)
{
    // Find next empty space or wall in direction of travel.
    auto row = robot.row + dir.row, col = robot.col + dir.col;
    while (grid[row][col] != '#' && grid[row][col] != '.')
        row += dir.row, col += dir.col;

    // Find next space in direction of travel.
    auto next_row = robot.row + dir.row, next_col = robot.col + dir.col;

    // Blocked?
    if (grid[row][col] == '#')
        return false;

    // Simple Move?
    if (grid[next_row][next_col] == '.')
    {
        grid[robot.row][robot.col] = '.';
        grid[robot.row + dir.row][robot.col + dir.col] = '@';
        robot.row = next_row, robot.col = next_col;
        return true;
    }

    // Else, it's a push
    grid[robot.row][robot.col] = '.';
    grid[robot.row + dir.row][robot.col + dir.col] = '@';
    robot.row = next_row, robot.col = next_col;
    grid[row][col] = 'O';
    return  true;
}

// See if it will be possible to move the object at (row, col) in the specified direction
// without actually performing the move.
auto can_move2(int row, int col, Direction dir)
{
    if (grid[row][col] == '.')
        return true;

    if (grid[row][col] == '#')
        return false;

    // Align with start of object, like all other cases
    if (grid[row][col] == ']')
        col--;

    auto next_row = row + dir.row, next_col = col + dir.col;
    if (grid[row][col] == '[' && dir.col == 1) next_col++;
    return
        can_move2(next_row, next_col, dir) &&
        (grid[row][col] != '[' || dir.row == 0 || can_move2(next_row, next_col + 1, dir));
}

// Move the object at the location in the direction indicated. Should first call can_move2() 
// to see whether the way is open or not. Otherwise, this function would have to back out
// partial changes. TODO: Make this one function with a "test" bool parameter since the
// traversal actions are essentially the same.
auto move2(int row, int col, Direction dir)
{
    if (grid[row][col] == '.')
        return true;

    if (grid[row][col] == '#')
        return false;

    // Align with start of a double-wide object, to match all other cases
    if (grid[row][col] == ']')
        col--;

    auto next_row = row + dir.row, next_col = col + dir.col;
    if (grid[row][col] == '[' && dir.col == 1) next_col++;

    if (move2(next_row, next_col, dir) &&
        (grid[row][col] != '[' || dir.row == 0 || move2(next_row, next_col + 1, dir)))
    {
        if (grid[row][col] == '[' && dir.col == 1) --next_col;

        // Special case for double-wide moves
        if (grid[row][col] == '[')
        {
            if (dir.row == 0) // horizontal moves
            {
                grid[next_row][next_col] = '[';
                grid[next_row][next_col + 1] = ']';
                if (dir.col == 1)
                    grid[row][col] = '.';
                else
                    grid[row - dir.row][col - dir.col] = '.';
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
            if (grid[row][col] == '@') robot.row = next_row, robot.col = next_col;
            grid[next_row][next_col] = grid[row][col];
            grid[row][col] = '.';
        }
    }

    return true;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line) && line.length())
    {
        auto at_pos = line.find('@');
        if (at_pos != std::string::npos)
            robot.row = int(grid.size()), robot.col = int(at_pos);
        grid.push_back(line);
    }

    auto move = ' ';
    while (file >> move)
    {
        Direction dir(move);
        do_move(dir);
    }

    return get_total_for_config();
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
            robot.row = int(grid.size()), robot.col = int(at_pos);
        grid.push_back(line2);
    }

    auto move = ' ';
    while (file >> move)
    {
        Direction dir(move);
        if (can_move2(robot.row, robot.col, dir))
            move2(robot.row, robot.col, dir);
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
