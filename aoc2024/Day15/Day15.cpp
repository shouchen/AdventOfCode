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

    if (grid[loc.row][loc.col] == ']')
        loc.col--; // align to box start if double-wide

    auto next = Location{ loc.row + dir.row, loc.col + dir.col };

    // double-wide box moves require additional checks
    if (grid[loc.row][loc.col] == '[')
    {
        auto next2 = Location{ next.row, next.col + 1 };

        // rightward double-wide move: the single cell to check is 2 away from loc
        if (dir.col == 1)
            return can_move(grid, next2, dir);

        // vertical double-wide moves have to check two cells - this one and the one below
        if (dir.row != 0 && !can_move(grid, next2, dir))
            return false;
    }

    return can_move(grid, next, dir);
}

Location do_move(Grid &grid, Location loc, Direction dir)
{
    if (grid[loc.row][loc.col] == '.' || grid[loc.row][loc.col] == '#') return loc;

    if (grid[loc.row][loc.col] == ']')
        loc.col--; // align to box start if double-wide

    auto next = Location{ loc.row + dir.row, loc.col + dir.col };

    // open up destination cells, then do actual box move
    if (grid[loc.row][loc.col] == '[')
    {
        auto next2 = Location{ next.row, next.col + 1 };

        if (dir.col == 1)
        {
            // rightward double-wide move: the single cell to open up is 2 away from loc
            do_move(grid, next2, dir);
        }
        else
        {
            // for all moves other than right, open up one adjacent cell in the direction of move
            do_move(grid, next, dir);

            // vertical double-wide moves also have to open up a second cell besides the above
            if (dir.row != 0)
                do_move(grid, next2, dir);
        }

        // actually do the double-wide move
        grid[next.row][next.col] = '[', grid[next2.row][next2.col] = ']';

        if (dir.col != -1) grid[loc.row][loc.col] = '.'; // for every move dir except left
        if (dir.col != 1)  grid[loc.row][loc.col + 1] = '.'; // for every move dir except right
    }
    else
    {
        // for single-wide, just free the adjacent cell in the direction of move
        if (grid[next.row][next.col] != '.')
            do_move(grid, next, dir);

        // actually do the single-wide move
        grid[next.row][next.col] = grid[loc.row][loc.col];
        grid[loc.row][loc.col] = '.';
    }

    return next;
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
            robot = do_move(grid, robot, dir);
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
