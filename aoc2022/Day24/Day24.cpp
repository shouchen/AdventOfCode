#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <set>
#include <numeric>
#include <cassert>

using RowCol = std::pair<int, int>;
using Grid = std::vector<std::string>;

std::vector<Grid> grids;
auto rows = 0, cols = 0;

void read_input(const std::string &filename)
{
    struct Blizzard
    {
        RowCol pos;
        char dir;
    };

    std::ifstream file(filename);
    std::string line;
    std::vector<Blizzard> blizzards;
    Grid grid;

    while (file >> line)
    {
        for (auto i = 0; i < line.length(); i++)
            if (line[i] != '#' && line[i] != '.')
            {
                blizzards.push_back({ { int(grid.size()), i }, line[i] });
                line[i] = '.';
            }

        grid.push_back(line);
    }

    rows = int(grid.size()), cols = int(grid[0].size());
    auto blizzard_period = std::lcm(rows - 2, cols - 2);

    for (auto i = 0; i < blizzard_period; i++)
    {
        auto grid_now = grid;

        for (auto &b : blizzards)
            grid_now[b.pos.first][b.pos.second] = '#';

        grids.push_back(grid_now);

        for (auto &b : blizzards)
        {
            auto &row = b.pos.first, &col = b.pos.second;

            switch (b.dir)
            {
            case '^': if (--row == 0)        row = rows - 2; break;
            case 'v': if (++row == rows - 1) row = 1;        break;
            case '<': if (--col == 0)        col = cols - 2; break;
            case '>': if (++col == cols - 1) col = 1;        break;
            }
        }
    }
}

auto get_fastest_time(RowCol start, RowCol end, int start_minute)
{
    struct State
    {
        RowCol pos;
        int minutes;
    };

    const auto blizzard_period = grids.size();
    auto fastest = INT_MAX;
    std::queue<State> q;
    std::set<std::pair<RowCol, int>> seen;

    q.push(State{ start, start_minute });

    while (!q.empty())
    {
        State state = q.front();
        q.pop();

        if (state.minutes >= fastest)
            continue;

        if (state.pos == end)
        {
            fastest = std::min(state.minutes, fastest);
            continue;
        }

        auto key = make_pair(state.pos, state.minutes); 
        if (seen.find(key) != seen.end())
            continue;

        seen.insert(key);

        Grid &grid = grids[(state.minutes + 1) % blizzard_period];

        State s = { { state.pos.first + 1, state.pos.second }, state.minutes + 1};
        if (s.pos.first <= rows - 1 && grid[s.pos.first][s.pos.second] == '.')
            q.push(s);

        s = { { state.pos.first, state.pos.second + 1 }, state.minutes + 1};
        if (grid[s.pos.first][s.pos.second] == '.')
            q.push(s);

        s = { { state.pos.first - 1, state.pos.second }, state.minutes + 1};
        if (s.pos.first >= 0 && grid[s.pos.first][s.pos.second] == '.')
            q.push(s);

        s = { { state.pos.first, state.pos.second - 1 }, state.minutes + 1};
        if (grid[s.pos.first][s.pos.second] == '.')
            q.push(s);

        s = { { state.pos.first, state.pos.second }, state.minutes + 1 };
        if (grid[state.pos.first][state.pos.second] == '.')
            q.push(s);
    }

    return fastest;
}

auto get_output()
{
    const auto top_left = RowCol{ 0, 1 };
    const auto bottom_right = RowCol{ rows - 1, cols - 2 };
    auto retval = std::make_pair(0, 0);

    retval.first = get_fastest_time(top_left, bottom_right, 0);
    retval.second = get_fastest_time(bottom_right, top_left, retval.first);
    retval.second = get_fastest_time(top_left, bottom_right, retval.second);

    return retval;
}

int main()
{
    read_input("input.txt");
    auto output = get_output();

    std::cout << "Part One: " << output.first << std::endl;
    std::cout << "Part Two: " << output.second << std::endl;

    assert(output.first == 283);
    assert(output.second == 883);
    return 0;
}