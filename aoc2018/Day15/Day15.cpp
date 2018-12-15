#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <string>
#include <numeric>
#include <algorithm>
#include <cassert>

struct Unit
{
    int row, col;
    int hit_points = 200;
    char type;
    Unit(int row, int col, char type, int hit_points) : row(row), col(col), type(type), hit_points(hit_points) {}
};

std::vector<std::string> grid;
std::vector<Unit *> units;

void dump()
{
    for (int row = 0; row < grid.size(); row++)
    {
        for (int col = 0; col < grid[row].size(); col++)
            std::cout << grid[row][col];

        bool first = true;
        for (int col = 0; col < grid[row].size(); col++)
            for (auto unit : units)
                if (unit->row == row && unit->col == col)
                {
                    std::cout << (first ? " " : ", ") << unit->type << "(" << unit->hit_points << ")";
                    first = false;
                }
 
        std::cout << std::endl;
    }
}

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
        grid.push_back(line);

    for (int row = 0; row < grid.size(); row++)
        for (int col = 0; col < grid[row].size(); col++)
        {
            if (grid[row][col] == 'E' || grid[row][col] == 'G')
                units.push_back(new Unit(row, col, grid[row][col], 200));
        }
}

std::vector<std::pair<int, int>> best_path;

void get_next_step(int row, int col, int &shortest, std::vector<std::pair<int, int>> &path)
{
    //std::cout << "STEP" << std::endl;
    //dump();

    if (path.size() >= shortest)
        return;

    if (grid[row][col] == '?')
    {
        shortest = path.size();
        best_path.clear();
        for (auto &item : path)
            best_path.push_back(item);
        return;
    }

    if (grid[row - 1][col] == '.' || grid[row - 1][col] == '?')
    {
        char temp = grid[--row][col];
        if (temp == '.')
            grid[row][col] = 'x';
        path.push_back(std::make_pair(row, col));
        get_next_step(row, col, shortest, path);
        path.pop_back();
        grid[row++][col] = temp;
    }
    if (grid[row][col - 1] == '.' || grid[row][col - 1] == '?')
    {
        char temp = grid[row][--col];
        if (temp == '.')
            grid[row][col] = 'x';
        path.push_back(std::make_pair(row, col));
        get_next_step(row, col, shortest, path);
        path.pop_back();
        grid[row][col++] = temp;
    }
    if (grid[row][col + 1] == '.' || grid[row][col + 1] == '?')
    {
        char temp = grid[row][++col];
        if (temp == '.')
            grid[row][col] = 'x';
        path.push_back(std::make_pair(row, col));
        get_next_step(row, col, shortest, path);
        path.pop_back();
        grid[row][col--] = temp;
    }
    if (grid[row + 1][col] == '.' || grid[row + 1][col] == '?')
    {
        char temp = grid[++row][col];
        if (temp == '.')
            grid[row][col] = 'x';
        path.push_back(std::make_pair(row, col));
        get_next_step(row, col, shortest, path);
        path.pop_back();
        grid[row--][col] = temp;
    }
}

bool do_turn(Unit *unit)
{
    // If already in a target square, don't move
    if (unit->type == 'E' && grid[unit->row - 1][unit->col] == 'G' || unit->type == 'G' && grid[unit->row - 1][unit->col] == 'E') return false;
    if (unit->type == 'E' && grid[unit->row][unit->col - 1] == 'G' || unit->type == 'G' && grid[unit->row][unit->col - 1] == 'E') return false;
    if (unit->type == 'E' && grid[unit->row][unit->col + 1] == 'G' || unit->type == 'G' && grid[unit->row][unit->col + 1] == 'E') return false;
    if (unit->type == 'E' && grid[unit->row + 1][unit->col] == 'G' || unit->type == 'G' && grid[unit->row + 1][unit->col] == 'E') return false;

    // Identify all open squares in range of each target
    for (auto other : units)
    {
        if (other->type != unit->type)
        {
            if (grid[other->row - 1][other->col] == '.') grid[other->row - 1][other->col] = '?';
            if (grid[other->row][other->col - 1] == '.') grid[other->row][other->col - 1] = '?';
            if (grid[other->row][other->col + 1] == '.') grid[other->row][other->col + 1] = '?';
            if (grid[other->row + 1][other->col] == '.') grid[other->row + 1][other->col] = '?';
        }
    }

    // Get shortest distance to a target
    std::vector<std::pair<int, int>> path;
    int shortest = std::numeric_limits<int>::max();
    best_path.clear();
    get_next_step(unit->row, unit->col, shortest, path);

    // Remove all the ?'s that were added
    for (int row = 0; row < grid.size(); row++)
        for (int col = 0; col < grid[row].size(); col++)
            if (grid[row][col] == '?')
                grid[row][col] = '.';

    // Take the best step (if any)
    if (!best_path.empty())
    {
        grid[unit->row][unit->col] = '.';
        unit->row = best_path[0].first;
        unit->col = best_path[0].second;
        grid[unit->row][unit->col] = unit->type;
        return true;
    }

    return false;
}

bool do_round()
{
    // Sort units
    std::sort(units.begin(), units.end(), [&](const Unit *lhs, const Unit *rhs) -> bool {
        return std::make_pair(lhs->row, rhs->col) < std::make_pair(rhs->row, rhs->col);
    });

    bool move_made = false;
    for (auto unit : units)
    {
        move_made |= do_turn(unit);
    }

    return move_made;
}

int main()
{
    read_input("input.txt");
    int round = 0;

    std::cout << "Initially: " << std::endl;
    dump();
    std::cout << std::endl;

    while (do_round())
    {
        round++;
        std::cout << "After " << round << " round(s)" << std::endl;
        dump();
        std::cout << std::endl;
    }

    return 0;
}
