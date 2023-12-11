#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <cassert>

using namespace std;

std::vector<std::string> grid;
void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}

auto do_part1()
{
    for (int row = 0; row < grid.size(); row++)
    {
        bool only_periods = true;
        for (int col = 0; col < grid[row].size(); col++)
        {
            if (grid[row][col] != '.')
            {
                only_periods = false;
                break;
            }
        }

        if (only_periods)
        {
            grid.push_back(string());
            for (int rr = grid.size() - 1; rr > row; rr--)
                grid[rr] = grid[rr - 1];
            row++;
        }
    }

    for (int col = 0; col < grid[0].size(); col++)
    {
        bool only_periods = true;
        for (int row = 0; row < grid.size(); row++)
        {
            if (grid[row][col] != '.')
            {
                only_periods = false;
                break;
            }
        }

        if (only_periods)
        {
            for (int row = 0; row < grid.size(); row++)
            {
                grid[row].push_back('.');
                for (int cc = grid[row].size() - 1; cc > col; cc--)
                    grid[row][cc] = grid[row][cc - 1];
            }
            col++;
        }
    }

    vector<pair<int, int>> galaxies;
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == '#')
                galaxies.push_back(make_pair(i, j));
        }

    int total = 0;
    for (int i = 0; i < galaxies.size(); i++)
        for (int j = i + 1; j < galaxies.size(); j++)
        {
            auto dist = abs(galaxies[i].first - galaxies[j].first) + abs(galaxies[i].second - galaxies[j].second);
            total += dist;
        }

    return total;
}

set<int> empty_rows, empty_cols;

long long get_part_two_dist(pair<int, int> &g1, pair<int, int> &g2)
{
    auto d1 = 0, d2 = 0;
    if (g1.first < g2.first) d1 = 1;
    if (g1.first > g2.first) d1 = -1;
    if (g1.second < g2.second) d2 = 1;
    if (g1.second > g2.second) d2 = -1;

    long long dist = 0;
    if (d1)
    {
        for (int row = g1.first; row != g2.first; row += d1)
        {
            dist += (empty_rows.find(row) == empty_rows.end()) ? 1 : 1000000;
        }
    }
    if (d2)
    {
        for (int col = g1.second; col != g2.second; col += d2)
        {
            dist += (empty_cols.find(col) == empty_cols.end()) ? 1 : 1000000;
        }
    }

    return dist;
}

auto do_part2()
{

    for (int row = 0; row < grid.size(); row++)
    {
        bool only_periods = true;
        for (int col = 0; col < grid[row].size(); col++)
        {
            if (grid[row][col] != '.')
            {
                only_periods = false;
                break;
            }
        }

        if (only_periods)
            empty_rows.insert(row);
    }

    for (int col = 0; col < grid[0].size(); col++)
    {
        bool only_periods = true;
        for (int row = 0; row < grid.size(); row++)
        {
            if (grid[row][col] != '.')
            {
                only_periods = false;
                break;
            }
        }

        if (only_periods)
            empty_cols.insert(col);
    }

    vector<pair<int, int>> galaxies;
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == '#')
                galaxies.push_back(make_pair(i, j));
        }

    long long total = 0;
    for (int i = 0; i < galaxies.size(); i++)
        for (int j = i + 1; j < galaxies.size(); j++)
        {
            //cout << i << " " << j << " --> ";
            //if (i == 0 && j == 6)
            //    cout << endl;
            auto dist = get_part_two_dist(galaxies[i], galaxies[j]);
            //cout << dist << endl;
            total += dist;
        }

    return total;
}

#define INPUT_FILE "test.txt"
#define INPUT_FILE "input.txt"

int main()
{
    read_grid(INPUT_FILE);



    //auto part1 = do_part1();
    //std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    //assert(part1 == 9536038);
    //assert(part2 == 447744640566);
    return 0;
}
