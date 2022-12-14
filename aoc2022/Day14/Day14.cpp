#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cassert>

using namespace std;

map<pair<int, int>, char> grid;
int min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;

void dump()
{
    for (int i = min_y; i <= max_y; i++)
    {
        for (int j = min_x; j <= max_x; j++)
            if (grid.find(make_pair(j, i)) != grid.end())
            {
                cout << grid[make_pair(j, i)];
            }
            else
                cout << ".";
        cout << endl;
    }
    cout << endl;
}

auto do_part1(const std::string &filename)
{
    grid.clear();
    min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;

    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        stringstream ss(line);
        int a, b, c, d;
        string arrow;
        char comma;

        ss >> a >> comma >> b;
        while (ss >> arrow >> c >> comma >> d)
        {
            if (a == c)
            {
                if (b < d)
                    for (int i = b; i <= d; i++)
                        grid[make_pair(a, i)] = '#';
                else
                    for (int i = b; i >= d; i--)
                        grid[make_pair(a, i)] = '#';
            }
            else
            {
                if (a < c)
                    for (int i = a; i <= c; i++)
                        grid[make_pair(i, b)] = '#';
                else
                    for (int i = a; i >= c; i--)
                        grid[make_pair(i, b)] = '#';
            }

            a = c, b = d;
        }
    }

    for (auto &i : grid)
    {
        min_x = min(min_x, i.first.first);
        max_x = max(max_x, i.first.first);
        min_y = min(min_y, i.first.second);
        max_y = max(max_y, i.first.second);
    }

    dump();

    for (int i = 0; i < 10000; i++)
    {
        auto sand = make_pair(500, 0);
        for (;;)
        {
            if (grid.find(make_pair(sand.first, sand.second + 1)) == grid.end())
            {
                sand.second++;
            }
            else if (grid.find(make_pair(sand.first - 1, sand.second + 1)) == grid.end())
            {
                sand.first--, sand.second++;
            }
            else if (grid.find(make_pair(sand.first + 1, sand.second + 1)) == grid.end())
            {
                sand.first++, sand.second++;
            }
            else
                break;

            if (sand.first < min_x || sand.first > max_x)
                return i;
        }

        grid[sand] = 'o';

        //dump();
    }
}

auto do_part2(const std::string &filename)
{
    grid.clear();
    min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;

    std::ifstream file(filename);
    std::string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        int a, b, c, d;
        string arrow;
        char comma;

        ss >> a >> comma >> b;
        while (ss >> arrow >> c >> comma >> d)
        {
            if (a == c)
            {
                if (b < d)
                    for (int i = b; i <= d; i++)
                        grid[make_pair(a, i)] = '#';
                else
                    for (int i = b; i >= d; i--)
                        grid[make_pair(a, i)] = '#';
            }
            else
            {
                if (a < c)
                    for (int i = a; i <= c; i++)
                        grid[make_pair(i, b)] = '#';
                else
                    for (int i = a; i >= c; i--)
                        grid[make_pair(i, b)] = '#';
            }

            a = c, b = d;
        }
    }

    for (auto &i : grid)
    {
        min_x = min(min_x, i.first.first);
        max_x = max(max_x, i.first.first);
        min_y = min(min_y, i.first.second);
        max_y = max(max_y, i.first.second);
    }

    dump();

    for (int i = -10000; i <= 10000; i++)
        grid[make_pair(i, max_y + 2)] = '#';

    for (int i = 0; i < 100000; i++)
    {
        auto sand = make_pair(500, 0);
        for (;;)
        {
            if (grid.find(make_pair(sand.first, sand.second + 1)) == grid.end())
            {
                sand.second++;
            }
            else if (grid.find(make_pair(sand.first - 1, sand.second + 1)) == grid.end())
            {
                sand.first--, sand.second++;
            }
            else if (grid.find(make_pair(sand.first + 1, sand.second + 1)) == grid.end())
            {
                sand.first++, sand.second++;
            }
            else
                break;
        }

        if (sand.first == 500 && sand.second == 0)
            return i + 1;

        grid[sand] = 'o';

        //dump();
    }
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 901);
    assert(part2 == 24589);
    return 0;
}
