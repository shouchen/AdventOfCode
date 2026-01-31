#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <string>
#include <cassert>

std::vector<std::string> grid(71, std::string(71, '.'));

auto get_shortest_distance()
{
    std::vector<std::vector<bool>> visited(71, std::vector<bool>(71, false));

    struct State { int x, y, cost; };
    std::queue<State> q;
    q.push({ 0, 0, 0 });

    while (!q.empty())
    {
        auto state = q.front(); q.pop();

        if (state.x == 70 && state.y == 70)
            return state.cost;

        if (visited[state.y][state.x])
            continue;
        visited[state.y][state.x] = true;

        if (state.x > 0 && grid[state.y][state.x - 1] != '#')
            q.push({ state.x - 1, state.y, state.cost + 1 });
        if (state.x < 70 && grid[state.y][state.x + 1] != '#')
            q.push({ state.x + 1, state.y, state.cost + 1 });

        if (state.y > 0 && grid[state.y - 1][state.x] != '#')
            q.push({ state.x, state.y - 1, state.cost + 1 });
        if (state.y < 70 && grid[state.y + 1][state.x] != '#')
            q.push({ state.x, state.y + 1, state.cost + 1 });
    }

    return INT_MAX;
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto x = 0, y = 0, num_bytes = 0;
    auto comma = ',';
    std::pair<int, std::string> retval;

    while (file >> x >> comma >> y)
    {
        grid[y][x] = '#';
        if (++num_bytes < 1024)
            continue;

        auto shortest = get_shortest_distance();

        if (num_bytes == 1024)
            retval.first = shortest;
        else if (shortest == INT_MAX)
        {
            std::ostringstream oss;
            oss << x << "," << y;
            retval.second = oss.str();
            break;
        }
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 506);
    assert(answer.second == "62,6");
    return 0;
}
