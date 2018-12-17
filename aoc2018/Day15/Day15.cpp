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
int rnd = 0;
int combat_ended_round = -1;
int highest_completed_round = -1;

void dump()
{
    for (int row = 0; row < grid.size(); row++)
    {
        for (int col = 0; col < grid[row].size(); col++)
            std::cout << grid[row][col];

        bool first = true;
        for (int col = 0; col < grid[row].size(); col++)
            for (auto unit : units)
                if (unit->hit_points > 0 && unit->row == row && unit->col == col)
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

struct BFS
{
    int row, col;
    int distance;
};

// 1. Find all shortest paths to a '?'.
// 2. Select the first one of these in reading order.
// 3. To another BFS to get from that one back to the start.
// 4. Select the one next to start that is first in reading order.
std::pair<int, int> get_next_step_bfs(int start_row, int start_col)
{
    std::map<std::pair<int, int>, int> dist_from_start;
    std::queue<BFS> q;

    q.push(BFS{ start_row, start_col, 0 });

    auto solution_distance = -1;
    std::vector<BFS> solutions;

    while (!q.empty())
    {
        auto curr = q.front();
        q.pop();

        auto row = curr.row;
        auto col = curr.col;
        auto distance = curr.distance;

        if (dist_from_start.find(std::make_pair(row, col)) != dist_from_start.end())
            continue;

        if (solution_distance > -1 && distance > solution_distance)
            continue;

//        std::cout << "Considering row=" << row << " and col=" << col << " with distance=" << distance << std::endl;

        dist_from_start[std::make_pair(row, col)] = distance;

        if (grid[row][col] == '?')
        {
            if (solution_distance == -1)
                solution_distance = distance;

            if (distance == solution_distance)
                solutions.push_back(BFS{ row, col, distance });
            continue;
        }

        if (grid[row - 1][col] == '.' || grid[row - 1][col] == '?' && dist_from_start.find(std::make_pair(row - 1, col)) == dist_from_start.end())
            q.push(BFS{ row - 1, col, distance + 1 });
        if (grid[row][col - 1] == '.' || grid[row][col - 1] == '?' && dist_from_start.find(std::make_pair(row, col - 1)) == dist_from_start.end())
            q.push(BFS{ row, col - 1, distance + 1 });
        if (grid[row][col + 1] == '.' || grid[row][col + 1] == '?' && dist_from_start.find(std::make_pair(row, col + 1)) == dist_from_start.end())
            q.push(BFS{ row, col + 1, distance + 1 });
        if (grid[row + 1][col] == '.' || grid[row + 1][col] == '?' && dist_from_start.find(std::make_pair(row + 1, col)) == dist_from_start.end())
            q.push(BFS{ row + 1, col, distance + 1 });
    }

    if (solutions.empty())
        return std::make_pair(-1, -1);

    // Sort solutions by row, col
    std::sort(solutions.begin(), solutions.end(), [&](const BFS &lhs, const BFS &rhs) -> bool {
        return std::make_pair(lhs.row, lhs.col) < std::make_pair(rhs.row, rhs.col);
    });

    // This is the selected In Range square to go to.
    auto row = solutions[0].row;
    auto col = solutions[0].col;
    auto distance = solutions[0].distance; auto part_one_distance = solutions[0].distance;

    // Now, do another (!) BFS to get back to the start
    std::map<std::pair<int, int>, int> dist_from_in_range;
    q.push(BFS{ row, col, 0 });
    bool done_second_bfs = false;

    while (!q.empty())
    {
        auto curr = q.front();
        q.pop();

        row = curr.row;
        col = curr.col;
        distance = curr.distance;

        if (dist_from_in_range.find(std::make_pair(row, col)) != dist_from_in_range.end())
            continue;

        dist_from_in_range[std::make_pair(row, col)] = distance;

        if (row == start_row && col == start_col)
        {
            done_second_bfs = true;
            break;
        }

        if (grid[row - 1][col] == '.' || (row - 1 == start_row && col == start_col) && dist_from_in_range.find(std::make_pair(row - 1, col)) == dist_from_in_range.end())
            q.push(BFS{ row - 1, col, distance + 1 });
        if (grid[row][col - 1] == '.' || (row == start_row && col - 1 == start_col) && dist_from_in_range.find(std::make_pair(row, col - 1)) == dist_from_in_range.end())
            q.push(BFS{ row, col - 1, distance + 1 });
        if (grid[row][col + 1] == '.' || (row == start_row && col + 1 == start_col) && dist_from_in_range.find(std::make_pair(row, col + 1)) == dist_from_in_range.end())
            q.push(BFS{ row, col + 1, distance + 1 });
        if (grid[row + 1][col] == '.' || (row + 1 == start_row && col == start_col) && dist_from_in_range.find(std::make_pair(row + 1, col)) == dist_from_in_range.end())
            q.push(BFS{ row + 1, col, distance + 1 });
    }

    // Now, take the first step in reading order
    auto retval = std::make_pair(-1, -1);


    auto up = std::make_pair(start_row - 1, start_col);
    if ((dist_from_in_range.find(up) != dist_from_in_range.end()) && dist_from_in_range[up] == part_one_distance - 1)
        retval = up;
    else
    {
        auto left = std::make_pair(start_row, start_col - 1);
        if ((dist_from_in_range.find(left) != dist_from_in_range.end()) && dist_from_in_range[left] == part_one_distance - 1)
            retval = left;
        else
        {
            auto right = std::make_pair(start_row, start_col + 1);
            if ((dist_from_in_range.find(right) != dist_from_in_range.end()) && dist_from_in_range[right] == part_one_distance - 1)
                retval = right;
            else
            {
                auto down = std::make_pair(start_row + 1, start_col);
                if ((dist_from_in_range.find(down) != dist_from_in_range.end()) && dist_from_in_range[down] == part_one_distance - 1)
                    retval = down;
            }
        }
    }

    best_path.clear();
    best_path.push_back(retval);
    return retval;

    //while (--distance)
    //{
    //    auto up = dist_from_start.find(std::make_pair(row - 1, col));
    //    auto left = dist_from_start.find(std::make_pair(row, col - 1));
    //    auto right = dist_from_start.find(std::make_pair(row, col + 1));
    //    auto down = dist_from_start.find(std::make_pair(row + 1, col));

    //    if (up != dist_from_start.end() && up->second == distance)
    //        row = up->first.first, col = up->first.second;
    //    else if (left != dist_from_start.end() && left->second == distance)
    //        row = left->first.first, col = left->first.second;
    //    else if (right != dist_from_start.end() && right->second == distance)
    //        row = right->first.first, col = right->first.second;
    //    else if (down != dist_from_start.end() && down->second == distance)
    //        row = down->first.first, col = down->first.second;
    //    else
    //        assert(false);
    //}

    //auto retval = std::make_pair(row, col);
    //best_path.push_back(retval);
    //return retval;
}

Unit *get_unit(int row, int col)
{
    for (auto u : units)
        if (u->hit_points > 0 && u->row == row && u->col == col)
            return u;
    return nullptr;
}

void do_attack(Unit *unit)
{
    Unit *other = nullptr;
    auto up = get_unit(unit->row - 1, unit->col);
    auto left = get_unit(unit->row, unit->col - 1);
    auto right = get_unit(unit->row, unit->col + 1);
    auto down = get_unit(unit->row + 1, unit->col);

    if (up && up->type == unit->type) up = nullptr;
    if (left && left->type == unit->type) left = nullptr;
    if (right && right->type == unit->type) right = nullptr;
    if (down && down->type == unit->type) down = nullptr;

    other = up;
    if (!other || left && left->hit_points < other->hit_points) other = left;
    if (!other || right && right->hit_points < other->hit_points) other = right;
    if (!other || down && down->hit_points < other->hit_points) other = down;

    if (other)
    {
        other->hit_points -= 3;
        if (other->hit_points <= 0)
        {
            grid[other->row][other->col] = '.';
        }
    }
}

bool do_turn(Unit *unit)
{
//    std::cout << "Doing turn for " << unit->type << "(" << unit->col << "," << unit->row << ")" << std::endl;
    if (unit->hit_points <= 0)
        return false;

    // If already in a target square, don't move
    if (unit->type == 'E' && grid[unit->row - 1][unit->col] == 'G' || unit->type == 'G' && grid[unit->row - 1][unit->col] == 'E')
    {
        do_attack(unit);
        return true;
    }
    if (unit->type == 'E' && grid[unit->row][unit->col - 1] == 'G' || unit->type == 'G' && grid[unit->row][unit->col - 1] == 'E')
    {
        do_attack(unit);
        return true;
    }
    if (unit->type == 'E' && grid[unit->row][unit->col + 1] == 'G' || unit->type == 'G' && grid[unit->row][unit->col + 1] == 'E')
    {
        do_attack(unit);
        return true;
    }
    if (unit->type == 'E' && grid[unit->row + 1][unit->col] == 'G' || unit->type == 'G' && grid[unit->row + 1][unit->col] == 'E')
    {
        do_attack(unit);
        return true;
    }

    // Identify all open squares in range of each target
    bool has_targets = false;
    for (auto other : units)
    {
        if (other->type != unit->type && other->hit_points > 0)
        {
            if (grid[other->row - 1][other->col] == '.')
                grid[other->row - 1][other->col] = '?';
            if (grid[other->row][other->col - 1] == '.')
                grid[other->row][other->col - 1] = '?';
            if (grid[other->row][other->col + 1] == '.')
                grid[other->row][other->col + 1] = '?';
            if (grid[other->row + 1][other->col] == '.')
                grid[other->row + 1][other->col] = '?';
            has_targets = true;
        }
    }

    if (!has_targets)
    {
        combat_ended_round = rnd;
    }

    // Get shortest distance to a target, using the 2 BFS steps described there
    best_path.clear();
    get_next_step_bfs(unit->row, unit->col);

    // Remove all the ?'s that were added
    for (int row = 0; row < grid.size(); row++)
        for (int col = 0; col < grid[row].size(); col++)
            if (grid[row][col] == '?')
                grid[row][col] = '.';

    // Take the best step (if any)
    if (best_path.empty())
        return false;

    grid[unit->row][unit->col] = '.';
    unit->row = best_path[0].first;
    unit->col = best_path[0].second;
    grid[unit->row][unit->col] = unit->type;

    // And do attack now if able
    if (unit->type == 'E' && grid[unit->row - 1][unit->col] == 'G' || unit->type == 'G' && grid[unit->row - 1][unit->col] == 'E')
    {
        do_attack(unit);
        return true;
    }
    if (unit->type == 'E' && grid[unit->row][unit->col - 1] == 'G' || unit->type == 'G' && grid[unit->row][unit->col - 1] == 'E')
    {
        do_attack(unit);
        return true;
    }
    if (unit->type == 'E' && grid[unit->row][unit->col + 1] == 'G' || unit->type == 'G' && grid[unit->row][unit->col + 1] == 'E')
    {
        do_attack(unit);
        return true;
    }
    if (unit->type == 'E' && grid[unit->row + 1][unit->col] == 'G' || unit->type == 'G' && grid[unit->row + 1][unit->col] == 'E')
    {
        do_attack(unit);
        return true;
    }

    return true;
}

bool do_round()
{
    rnd++;

    // Remove units with no hit_points
    units.erase(
        std::remove_if(units.begin(), units.end(), [&](const Unit *item) { return item->hit_points <= 0; }),
        units.end());

    // Sort remaining units
    std::sort(units.begin(), units.end(), [&](const Unit *lhs, const Unit *rhs) -> bool {
        return std::make_pair(lhs->row, rhs->col) < std::make_pair(rhs->row, rhs->col);
    });

    bool did_turn = false;
    for (auto unit : units)
    {
        did_turn |= do_turn(unit);
    }

    if (combat_ended_round == -1)
        highest_completed_round = rnd;

    return did_turn;
}

unsigned do_part1(const std::string &filename)
{
    best_path.clear();
    grid.clear();
    units.clear();
    rnd = 0;
    combat_ended_round = -1;
    highest_completed_round = -1;

    read_input(filename);

    std::cout << filename << " Initially: " << std::endl;
    dump();
    std::cout << std::endl;

    while (combat_ended_round == -1)
    {
        do_round();

        std::cout << "After " << rnd << " round(s)" << std::endl;
        dump();
        std::cout << std::endl;
    }

    int total = 0;
    for (auto &unit : units)
    {
        if (unit->hit_points > 0)
            total += unit->hit_points;
    }

    auto part1 = highest_completed_round * total; // 213221 too high, 103698 too low, 212984 wrong

    std::cout << "Part 1: " << highest_completed_round << " * " << total << " = " << part1 << std::endl;
    return part1;
}

int main()
{
    //auto a1 = do_part1("18740.txt");
    //auto a2 = do_part1("28944.txt");
    //auto a3 = do_part1("27755.txt");
    //auto a4 = do_part1("27730.txt");
    //auto a5 = do_part1("36334.txt");
    auto a6 = do_part1("39514.txt");

    //std::cout << "Ex #1: Answer=18740, me=" << a1 << ((a1 == 18740) ? "" : " WRONG!") << std::endl;
    //std::cout << "Ex #2: Answer=28944, me=" << a2 << ((a2 == 28944) ? "" : " WRONG!") << std::endl;
    //std::cout << "Ex #3: Answer=27755, me=" << a3 << ((a3 == 27755) ? "" : " WRONG!") << std::endl;
    //std::cout << "Ex #4: Answer=27730, me=" << a4 << ((a4 == 27730) ? "" : " WRONG!") << std::endl;
    //std::cout << "Ex #5: Answer=36334, me=" << a5 << ((a5 == 36334) ? "" : " WRONG!") << std::endl;
    std::cout << "Ex #6: Answer=39514, me=" << a6 << ((a6 == 39514) ? "" : " WRONG!") << std::endl;

    //read_input("39514.txt");

    //std::cout << "Initially: " << std::endl;
    //dump();
    //std::cout << std::endl;

    //while (combat_ended_round == -1)
    //{
    //    do_round();

    //    std::cout << "After " << rnd << " round(s)" << std::endl;
    //        dump();
    //        std::cout << std::endl;
    //}

    //int total = 0;
    //for (auto &unit : units)
    //{
    //    if (unit->hit_points > 0)
    //        total += unit->hit_points;
    //}

    //auto part1 = highest_completed_round * total; // 213221 too high, 103698 too low, 212984 wrong, 210522 wrong

    //std::cout << "Part 1: " << highest_completed_round << " * " << total << " = " << part1 << std::endl;

    return 0;
}
