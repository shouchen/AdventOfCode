#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <queue>
#include <map>
#include <stack>
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <limits>
#include <cstdlib>
#include <cassert>

using namespace std;

using RowCol = pair<int, int>;

struct Blizzard
{
    RowCol pos;
    char dir;
};

vector<Blizzard> blizzards;

vector<string> grid;

vector<vector<string>> timegrid;

int gcd(int a, int b)
{
    if (b) while ((a %= b) && (b %= a));
    return b ? b : a;
}

int lcm(int a, int b)
{
    int temp = gcd(a, b);

    return temp ? (a / temp * b) : 0;
}

struct State
{
    RowCol pos;
    int minutes;
};

void xdump(State &state, int blizzard_period)
{
    for (int i = 0; i < timegrid[state.minutes % blizzard_period].size(); i++)
    {
        for (int j = 0; j < timegrid[state.minutes % blizzard_period][i].size(); j++)
        {
            if (state.pos.first == i && state.pos.second == j)
                cout << 'E';
            else
                cout << timegrid[state.minutes % blizzard_period][i][j];
        }
        cout << endl;
    }

    cout << endl;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (getline(file, line))
    {
        for (int i = 0; i < line.length(); i++)
            if (line[i] == '>' || line[i] == '<' || line[i] == '^' || line[i] == 'v')
            {
                blizzards.push_back(Blizzard{ make_pair(grid.size(), i), line[i] });
                line[i] = '.';
            }
        grid.push_back(line);
    }

    auto blizzard_period = lcm(grid.size() - 2, grid[0].size() - 2);

    for (int i = 0; i < blizzard_period; i++)
    {
        auto grid_now = grid;

        for (auto &b : blizzards)
        {
            auto row = b.pos.first, col = b.pos.second;
            if (grid_now[row][col] == '.')
                grid_now[row][col] = b.dir;
            else if (isdigit(grid_now[row][col]))
                grid_now[row][col]++;
            else
                grid_now[row][col] = '2';
        }

        timegrid.push_back(grid_now);

        for (auto &b : blizzards)
        {
            auto &row = b.pos.first, &col = b.pos.second;
            switch (b.dir)
            {
            case '^':
                row--;
                if (row == 0 && grid[row][col] == '#')
                    row = grid.size() - 2;
                break;
            case 'v':
                row++;
                if (row == grid.size() - 1 && grid[row][col] == '#')
                    row = 1;
                break;
            case '<':
                col--;
                if (col == 0)
                    col = grid[row].size() - 2;
                break;
            case '>':
                col++;
                if (col == grid[row].size() - 1)
                    col = 1;
                break;
            default:
                assert(false);
            }
        }
    }

#if 0
    for (int i = 0; i < blizzard_period; i++)
    {
        cout << "@" << i << ":" << endl;
        for (auto &row : timegrid[i])
            cout << row << endl;
        cout << endl;
    }
#endif

    queue<State> q;
    q.push(State{ {0,1}, 0 });

    int best = INT_MAX;

    set<pair<RowCol, int>> seen; // pos + minutes % blizzard_period

    while (!q.empty())
    {
        State state = q.front();
        q.pop();

        //xdump(state, blizzard_period);

        if (state.minutes >= best)
            continue;

        if (state.pos.first == grid.size() - 1)
        {
            if (state.minutes < best)
            {
                best = state.minutes;
                cout << "BEST=" << best << endl;
                //xdump(state, blizzard_period);
            }

            continue;
        }

        // SEEN minute LCM and my row/col
        auto key = make_pair(state.pos, state.minutes % blizzard_period);
        if (seen.find(key) != seen.end())
            continue;

        seen.insert(key);

        state.minutes++;

        if (timegrid[state.minutes % blizzard_period][state.pos.first + 1][state.pos.second] == '.')
        {
            State s = state;
            s.pos.first++;
            q.push(s);
        }

        if (timegrid[state.minutes % blizzard_period][state.pos.first][state.pos.second + 1] == '.')
        {
            State s = state;
            s.pos.second++;
            q.push(s);
        }

        if (state.pos.first > 1 && timegrid[state.minutes % blizzard_period][state.pos.first - 1][state.pos.second] == '.')
        {
            State s = state;
            s.pos.first--;
            q.push(s);
        }

        if (timegrid[state.minutes % blizzard_period][state.pos.first][state.pos.second - 1] == '.')
        {
            State s = state;
            s.pos.second--;
            q.push(s);
        }

        if (timegrid[state.minutes % blizzard_period][state.pos.first][state.pos.second] == '.')
        {
            State s = state;
            q.push(s);
        }
    }

    return 0;
}

auto part2_prep_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (getline(file, line))
    {
        for (int i = 0; i < line.length(); i++)
            if (line[i] == '>' || line[i] == '<' || line[i] == '^' || line[i] == 'v')
            {
                blizzards.push_back(Blizzard{ make_pair(grid.size(), i), line[i] });
                line[i] = '.';
            }
        grid.push_back(line);
    }

    auto blizzard_period = lcm(grid.size() - 2, grid[0].size() - 2);

    for (int i = 0; i < blizzard_period; i++)
    {
        auto grid_now = grid;

        for (auto &b : blizzards)
        {
            auto row = b.pos.first, col = b.pos.second;
            if (grid_now[row][col] == '.')
                grid_now[row][col] = b.dir;
            else if (isdigit(grid_now[row][col]))
                grid_now[row][col]++;
            else
                grid_now[row][col] = '2';
        }

        timegrid.push_back(grid_now);

        for (auto &b : blizzards)
        {
            auto &row = b.pos.first, &col = b.pos.second;
            switch (b.dir)
            {
            case '^':
                row--;
                if (row == 0 && grid[row][col] == '#')
                    row = grid.size() - 2;
                break;
            case 'v':
                row++;
                if (row == grid.size() - 1 && grid[row][col] == '#')
                    row = 1;
                break;
            case '<':
                col--;
                if (col == 0)
                    col = grid[row].size() - 2;
                break;
            case '>':
                col++;
                if (col == grid[row].size() - 1)
                    col = 1;
                break;
            default:
                assert(false);
            }
        }
    }
}

auto do_part2(RowCol start, RowCol end, int minute_start)
{
    auto blizzard_period = timegrid.size();

#if 0
    for (int i = 0; i < blizzard_period; i++)
    {
        cout << "@" << i << ":" << endl;
        for (auto &row : timegrid[i])
            cout << row << endl;
        cout << endl;
    }
#endif

    queue<State> q;
    q.push(State{ start, minute_start });

    int best = INT_MAX;

    set<pair<RowCol, int>> seen; // pos + minutes % blizzard_period

    while (!q.empty())
    {
        State state = q.front();
        q.pop();

        //xdump(state, blizzard_period);

        if (state.minutes >= best)
            continue;

        if (state.pos == end)
        {
            if (state.minutes < best)
            {
                best = state.minutes;
                cout << "BEST=" << best << endl;
                //xdump(state, blizzard_period);
            }

            continue;
        }

        // SEEN minute LCM and my row/col
        auto key = make_pair(state.pos, state.minutes % blizzard_period);
        if (seen.find(key) != seen.end())
            continue;

        seen.insert(key);

        state.minutes++;

        if (start.first == 0)
        {
            if (timegrid[state.minutes % blizzard_period][state.pos.first + 1][state.pos.second] == '.')
            {
                State s = state;
                s.pos.first++;
                q.push(s);
            }

            if (timegrid[state.minutes % blizzard_period][state.pos.first][state.pos.second + 1] == '.')
            {
                State s = state;
                s.pos.second++;
                q.push(s);
            }

            if (state.pos.first > 1 && timegrid[state.minutes % blizzard_period][state.pos.first - 1][state.pos.second] == '.')
            {
                State s = state;
                s.pos.first--;
                q.push(s);
            }

            if (timegrid[state.minutes % blizzard_period][state.pos.first][state.pos.second - 1] == '.')
            {
                State s = state;
                s.pos.second--;
                q.push(s);
            }
        }
        else
        {
            if (timegrid[state.minutes % blizzard_period][state.pos.first - 1][state.pos.second] == '.')
            {
                State s = state;
                s.pos.first--;
                q.push(s);
            }

            if (timegrid[state.minutes % blizzard_period][state.pos.first][state.pos.second - 1] == '.')
            {
                State s = state;
                s.pos.second--;
                q.push(s);
            }

            if (state.pos.first < grid.size() - 1 && timegrid[state.minutes % blizzard_period][state.pos.first + 1][state.pos.second] == '.')
            {
                State s = state;
                s.pos.first++;
                q.push(s);
            }

            if (timegrid[state.minutes % blizzard_period][state.pos.first][state.pos.second + 1] == '.')
            {
                State s = state;
                s.pos.second++;
                q.push(s);
            }
        }

        if (timegrid[state.minutes % blizzard_period][state.pos.first][state.pos.second] == '.')
        {
            State s = state;
            q.push(s);
        }
    }

    return 0;
}


int main()
{
    part2_prep_data("input.txt");

    auto top_left = make_pair(0, 1);
    auto bottom_right = make_pair(grid.size() - 1, grid[0].size() - 2);

    do_part2(top_left, bottom_right, 0); // takes 283
    do_part2(bottom_right, top_left, 283); // up to 591
    do_part2(top_left, bottom_right, 591); // total is 883

    //auto part1 = process_data("test.txt", false);
    //std::cout << "Part One: " << part1 << std::endl;

    //auto part2 = process_data("test.txt", true);
    //std::cout << "Part Two: " << part2 << std::endl;

    //assert(part1 == 283);
    //assert(part2 == "PQTJRSHWS");
    return 0;
}
