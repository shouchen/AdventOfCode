#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

using Grid = std::vector<std::string>;

struct Location { int row = 0, col = 0; };
struct Direction { int row, col; Direction(char move); };

Direction::Direction(char move)
{
    switch (move)
    {
        case '^': row = -1, col =  0; break;
        case 'v': row =  1, col =  0; break;
        case '<': row =  0, col = -1; break;
        case '>': row =  0, col =  1; break;
        default:  row =  0, col =  0;
    }
}

auto can_move(const Grid &grid, Location loc, Direction dir)
{
    if (grid[loc.row][loc.col] == '.') return true;
    if (grid[loc.row][loc.col] == '#') return false;

    // Align with start of object if second half of double-wide
    if (grid[loc.row][loc.col] == ']')
        loc.col--;

    auto next = Location{ loc.row + dir.row, loc.col + dir.col };
    if (grid[loc.row][loc.col] == '[' && dir.col == 1) next.col++;

    return
        can_move(grid, next, dir) &&
        (grid[loc.row][loc.col] != '[' || dir.row == 0 || can_move(grid, Location{ next.row, next.col + 1 }, dir));
}

void do_move(Grid &grid, Location loc, Direction dir, Location &robot)
{
    // Align with start of object if second half of double-wide
    if (grid[loc.row][loc.col] == ']')
        loc.col--;

    auto next_row = loc.row + dir.row, next_col = loc.col + dir.col;
    if (grid[loc.row][loc.col] == '[' && dir.col == 1) next_col++;

    if (grid[next_row][next_col] != '.')
        do_move(grid, Location{ next_row, next_col }, dir, robot);

    if (grid[loc.row][loc.col] == '[' && dir.row != 0 && grid[next_row][next_col + 1] != '.')
        do_move(grid, Location{ next_row, next_col + 1 }, dir, robot);

    if (grid[loc.row][loc.col] == '[' && dir.col == 1)
        --next_col;

    if (grid[loc.row][loc.col] == '[') // double-wide moves
    {
        grid[next_row][next_col] = '[', grid[next_row][next_col + 1] = ']';

        if (dir.row == 0) // horizontal moves
        {
            if (dir.col == 1)
                grid[loc.row - dir.row][loc.col] = '.';
            else
                grid[loc.row - dir.row][loc.col - dir.col] = '.';
        }
        else // vertical moves
        {
            grid[loc.row][loc.col] = '.';
            grid[loc.row][loc.col + 1] = '.';
        }
    }
    else // single-wide moves
    {
        if (grid[loc.row][loc.col] == '@')
            robot = Location{ next_row, next_col };

        grid[next_row][next_col] = grid[loc.row][loc.col];
        grid[loc.row][loc.col] = '.';
    }
}

auto expand_line(const std::string &line)
{
    std::string retval;
    for (auto c : line)
        switch (c)
        {
            case '@': retval.push_back(c),   retval.push_back('.'); break;
            case 'O': retval.push_back('['), retval.push_back(']'); break;
            default:  retval.push_back(c),   retval.push_back(c);
        }

    return retval;
}

auto solve(const std::string &filename, bool expand)
{
    Grid grid;
    Location robot;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line) && line.length())
    {
        if (expand)
            line = expand_line(line);
    
        auto at_pos = line.find('@');
        if (at_pos != std::string::npos)
            robot = { int(grid.size()), int(at_pos) };
        grid.push_back(line);
    }

    auto move = ' ';
    while (file >> move)
    {
        Direction dir(move);
        if (can_move(grid, robot, dir))
            do_move(grid, robot, dir, robot);
    }

    auto gps_sum = 0;
    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].size(); col++)
            if (grid[row][col] == 'O' || grid[row][col] == '[')
                gps_sum += row * 100 + col;

    return gps_sum;
}

int main()
{
    auto part1 = solve("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 1438161);

    auto part2 = solve("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 1437981);

    return 0;
}
