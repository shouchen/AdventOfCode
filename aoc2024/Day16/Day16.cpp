#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <cassert>

struct Location { int row = 0, col = 0; };
struct Direction { int row = 0, col = 0; };

std::vector<std::string> grid;

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    Location start, end;

    while (file >> line)
    {
        auto s = line.find('S'), e = line.find('E');
        if (s != std::string::npos) start = { int(grid.size()), int(s) }, line[s] = '.';
        if (e != std::string::npos) end = { int(grid.size()), int(e) }, line[e] = '.';

        grid.push_back(line);
    }

    Location loc = start;
    Direction dir{ 0, 1 };

    std::vector<std::vector<int>> min_cost(grid.size(), std::vector<int>(grid[0].size(), INT_MAX));

    struct State
    {
        Location loc;
        Direction dir;
        int cost;
    };

    std::queue<State> q;
    q.push(State{ loc, dir, 0 });

    while (!q.empty())
    {
        State state = q.front(); q.pop();
        auto new_state = state;

        if (min_cost[state.loc.row][state.loc.col] <= state.cost)
            continue;
        min_cost[state.loc.row][state.loc.col] = state.cost;

        // up
        if (state.dir.row != 1)
        {
            new_state = state;
            --new_state.loc.row;
            if (new_state.dir.row == -1)
                new_state.cost += 1;
            else
                new_state.dir = { -1, 0 }, new_state.cost += 1001;

            if (grid[state.loc.row - 1][state.loc.col] != '#')
                q.push(new_state);
        }

        // down
        if (state.dir.row != -1)
        {
            new_state = state;
            new_state.loc.row++;
            if (new_state.dir.row == 1)
                new_state.cost += 1;
            else
                new_state.dir = { 1, 0 }, new_state.cost += 1001;

            if (grid[state.loc.row + 1][state.loc.col] != '#')
                q.push(new_state);
        }

        // left
        if (state.dir.col != 1)
        {
            new_state = state;
            --new_state.loc.col;
            if (new_state.dir.col == -1)
                new_state.cost += 1;
            else
                new_state.dir = { 0, -1 }, new_state.cost += 1001;

            if (grid[state.loc.row][state.loc.col - 1] != '#')
                q.push(new_state);
        }

        // right
        if (state.dir.col != -1)
        {
            new_state = state;
            new_state.loc.col++;
            if (new_state.dir.col == 1)
                new_state.cost += 1;
            else
                new_state.dir = { 0, 1 }, new_state.cost += 1001;

            if (grid[state.loc.row][state.loc.col + 1] != '#')
                q.push(new_state);
        }
    }

    return min_cost[end.row][end.col];
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
        ;

    return -1;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 101492);

    //auto part2 = do_part2("input.txt");
    //std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
