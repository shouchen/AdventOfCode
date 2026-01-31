#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <cassert>

// Real input will be 71x71, test is 7x7   (so SIZE 7 or 71)
#define SIZE 71

std::vector<std::string> grid(SIZE, std::string(SIZE, '.'));

void dump()
{
    for (auto &row : grid)
    {
        for (auto c : row)
            std::cout << (c ? c : '.');
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

auto get_dist()
{
    std::vector<std::vector<bool>> visited(SIZE, std::vector<bool>(SIZE, false));

    struct State { int x, y, cost; };
    std::queue<State> q;
    q.push({ 0, 0, 0 });

    while (!q.empty())
    {
        auto state = q.front(); q.pop();

        if (state.x == SIZE - 1 && state.y == SIZE - 1)
            return state.cost;

        if (visited[state.y][state.x])
            continue;
        visited[state.y][state.x] = true;

        if (state.x > 0 && grid[state.y][state.x - 1] != '#')
            q.push({ state.x - 1, state.y, state.cost + 1 });
        if (state.x < SIZE - 1 && grid[state.y][state.x + 1] != '#')
            q.push({ state.x + 1, state.y, state.cost + 1 });

        if (state.y > 0 && grid[state.y - 1][state.x] != '#')
            q.push({ state.x, state.y - 1, state.cost + 1 });
        if (state.y < SIZE - 1 && grid[state.y + 1][state.x] != '#')
            q.push({ state.x, state.y + 1, state.cost + 1 });
    }

    return -1;
}

auto do_part1(const std::string &filename, int bytes)
{
    std::ifstream file(filename);
    std::string line;
    auto x = 0, y = 0;
    auto comma = ',';

    auto debug = 0;
    while (file >> x >> comma >> y)
    {
        grid[y][x] = '#';
        if (++debug == bytes) break;
    }

    //dump();

    return get_dist();
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto x = 0, y = 0;
    auto comma = ',';

    while (file >> x >> comma >> y)
    {
        grid[y][x] = '#';
        if (get_dist() == -1)
        {
            std::cout << x << "," << y << std::endl;
            break;
        }
    }

    //dump();

    return -1;
}

int main()
{
    auto part1 = do_part1("input.txt", 1024);
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 506);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == "62,6");

    return 0;
}
