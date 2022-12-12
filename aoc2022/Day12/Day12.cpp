#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <cassert>

auto do_part1(const std::string &filename)
{
    std::vector<std::string> grid;
    std::set<std::pair<int, int>> visited;
    std::pair<int, int> startpos, endpos;

    std::ifstream file(filename);
    std::string line;

    while (getline(file, line))
    {
        auto temp = line.find('S');
        if (temp != -1)
        {
            line[temp] = 'a';
            startpos = std::make_pair(grid.size(), temp);
        }

        temp = line.find('E');
        if (temp != -1)
        {
            line[temp] = 'z';
            endpos = std::make_pair(grid.size(), temp);
        }

        grid.push_back(line);
    }

    std::map<std::pair<int, int>, int> dist;
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[i].length(); j++)
            dist[std::make_pair(i,j)] = INT_MAX;
    dist[startpos] = 0;

    for (int d = 0; d < 100000; d++)
    {
        for (int i = 0; i < grid.size(); i++)
            for (int j = 0; j < grid[i].length(); j++)
            {
                auto temp = dist[std::make_pair(i, j)];
                if (temp == d)
                {
                    if (i > 0 &&
                        dist[std::make_pair(i - 1, j)] > d + 1 &&
                        grid[i - 1][j] - grid[i][j] <= 1)
                    {
                        dist[std::make_pair(i - 1, j)] = d + 1;
                    }
                    if (i < grid.size() - 1 &&
                        dist[std::make_pair(i + 1, j)] > d + 1 &&
                        grid[i + 1][j] - grid[i][j] <= 1)
                    {
                        dist[std::make_pair(i + 1, j)] = d + 1;
                    }
                    if (j > 0 &&
                        dist[std::make_pair(i, j - 1)] > d + 1 &&
                        grid[i][j - 1] - grid[i][j] <= 1)
                    {
                        dist[std::make_pair(i, j - 1)] = d + 1;
                    }
                    if (j < grid[i].length() - 1 &&
                        dist[std::make_pair(i, j + 1)] > d + 1 &&
                        grid[i][j + 1] - grid[i][j] <= 1)
                    {
                        dist[std::make_pair(i, j + 1)] = d + 1;
                    }
                }
            }

        if (dist[endpos] != INT_MAX)
            return dist[endpos];
    }

    return -1;
}

auto do_part2(const std::string &filename)
{
    std::vector<std::string> grid;
    std::set<std::pair<int, int>> visited;
    std::pair<int, int> startpos, endpos;

    std::ifstream file(filename);
    std::string line;

    while (getline(file, line))
    {
        auto temp = line.find('S');
        if (temp != -1)
        {
            line[temp] = 'a';
            startpos = std::make_pair(grid.size(), temp);
        }

        temp = line.find('E');
        if (temp != -1)
        {
            line[temp] = 'z';
            endpos = std::make_pair(grid.size(), temp);
        }

        grid.push_back(line);
    }

    std::map<std::pair<int, int>, int> dist;
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[i].length(); j++)
            dist[std::make_pair(i, j)] = INT_MAX;
    dist[endpos] = 0;

    for (int d = 0; d < 100000; d++)
    {
        for (int i = 0; i < grid.size(); i++)
            for (int j = 0; j < grid[i].length(); j++)
            {
                auto temp = dist[std::make_pair(i, j)];
                if (temp == d)
                {
                    if (i > 0 &&
                        dist[std::make_pair(i - 1, j)] > d + 1 &&
                        grid[i][j] - grid[i - 1][j] <= 1)
                    {
                        dist[std::make_pair(i - 1, j)] = d + 1;
                        if (grid[i - 1][j] == 'a')
                            return d + 1;
                    }
                    if (i < grid.size() - 1 &&
                        dist[std::make_pair(i + 1, j)] > d + 1 &&
                        grid[i][j] - grid[i + 1][j] <= 1)
                    {
                        dist[std::make_pair(i + 1, j)] = d + 1;
                        if (grid[i + 1][j] == 'a')
                            return d + 1;
                    }
                    if (j > 0 &&
                        dist[std::make_pair(i, j - 1)] > d + 1 &&
                        grid[i][j] - grid[i][j - 1] <= 1)
                    {
                        dist[std::make_pair(i, j - 1)] = d + 1;
                        if (grid[i][j - 1] == 'a')
                            return d + 1;
                    }
                    if (j < grid[i].length() - 1 &&
                        dist[std::make_pair(i, j + 1)] > d + 1 &&
                        grid[i][j] - grid[i][j + 1] <= 1)
                    {
                        dist[std::make_pair(i, j + 1)] = d + 1;
                        if (grid[i][j + 1] == 'a')
                            return d + 1;
                    }
                }
            }

        // part1
        if (dist[startpos] != INT_MAX)
            return dist[startpos];
    }

    return -1;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 408);
    assert(part2 == 399);
    return 0;
}
