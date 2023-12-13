#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <ctype.h>
#include <algorithm>
#include <numeric>
#include <regex>
#include <cassert>

using namespace std;

std::vector<std::string> grid;
void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    grid.clear();
    while (std::getline(file, line) && !line.empty())
        grid.push_back(line);
}

vector<int> row, col;

int find_vref(int skip)
{
    for (int i = 0; i < grid[0].size() - 1; i++)
    {
        if (i == skip)
            continue;

        bool reflect = true;
        for (int a = i, b = i + 1; a >= 0 && b < grid[0].size(); a--, b++)
            if (col[a] != col[b])
            {
                reflect = false;
                break;
            }

        if (reflect)
            return i;
    }

    return -1;
}

int find_href(int skip)
{
    for (int i = 0; i < grid.size() - 1; i++)
    {
        if (i == skip)
            continue;

        bool reflect = true;
        for (int a = i, b = i + 1; a >= 0 && b < grid.size(); a--, b++)
            if (row[a] != row[b])
            {
                reflect = false;
                break;
            }

        if (reflect)
            return i;
    }

    return -1;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    long long retval = -1;

    while (!file.eof())
    {
        grid.clear();
        row.clear();
        col.clear();
        while (std::getline(file, line) && !line.empty())
            grid.push_back(line);

        while (row.size() < grid.size())
            row.push_back(0);
        while (col.size() < grid[0].size())
            col.push_back(0);

        // cols
        for (int i = 0; i < grid[0].size(); i++)
        {
            for (int j = 0; j < grid.size(); j++)
            {
                if (grid[j][i] == '#')
                    col[i] |= (1 << j);
            }
        }

        // rows
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[i].size(); j++)
            {
                if (grid[i][j] == '#')
                    row[i] |= (1 << j);
            }
        }

        // vertical ref
        auto vref = find_vref(0) + 1; // one based
        auto href = find_href(0) + 1; // one based
        auto x = vref + 100 * href;
        cout << x << endl;

        retval += x;
        cout << " Total=" << retval << endl;
    }

    return retval;
}

int do_smudge(int vref, int href)
{
    for (auto srow = 0; srow < grid.size(); srow++)
    {
        for (auto scol = 0; scol < grid[srow].size(); scol++)
        {
            if (grid[srow][scol] == '.')
                grid[srow][scol] = '#';
            else if (grid[srow][scol] == '#')
                grid[srow][scol] = '.';
            else
                assert(false);

            row.clear();
            col.clear();
            while (row.size() < grid.size())
                row.push_back(0);
            while (col.size() < grid[0].size())
                col.push_back(0);

            // cols
            for (int i = 0; i < grid[0].size(); i++)
            {
                for (int j = 0; j < grid.size(); j++)
                {
                    if (grid[j][i] == '#')
                        col[i] |= (1 << j);
                }
            }

            // rows
            for (int i = 0; i < grid.size(); i++)
            {
                for (int j = 0; j < grid[i].size(); j++)
                {
                    if (grid[i][j] == '#')
                        row[i] |= (1 << j);
                }
            }

            if (srow == 6 && scol == grid[6].size() - 1)
                cout << endl;

            // vertical ref
            auto vref2 = find_vref(vref - 1) + 1; // one based
            auto href2 = find_href(href - 1) + 1; // one based

            if (vref2 > 0)
            {
                cout << "vref2 " << vref2 << endl;
                return vref2;
            }
            else if (href2 > 0)
            {
                cout << "href2 " << href2 << endl;
                return 100 * href2;
            }

            if (grid[srow][scol] == '.')
                grid[srow][scol] = '#';
            else if (grid[srow][scol] == '#')
                grid[srow][scol] = '.';
            else
                assert(false);
        }
    }

    assert(false);
    return 0; 
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    long long retval = 0;

    while (!file.eof())
    {
        cout << "---" << endl;
        grid.clear();
        while (std::getline(file, line) && !line.empty())
            grid.push_back(line);

        row.clear();
        col.clear();
        while (row.size() < grid.size())
            row.push_back(0);
        while (col.size() < grid[0].size())
            col.push_back(0);

        // cols
        for (int i = 0; i < grid[0].size(); i++)
        {
            for (int j = 0; j < grid.size(); j++)
            {
                if (grid[j][i] == '#')
                    col[i] |= (1 << j);
            }
        }

        // rows
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[i].size(); j++)
            {
                if (grid[i][j] == '#')
                    row[i] |= (1 << j);
            }
        }

        // vertical ref
        auto vref = find_vref(-1) + 1; // one based
        auto href = find_href(-1) + 1; // one based
        
        // part2
        auto smudge = do_smudge(vref, href);
        retval += smudge;
    }

    return retval;
}

#define INPUT_FILE "test.txt"
#define INPUT_FILE "input.txt"

int main()
{
    //auto part1 = do_part1(INPUT_FILE);
    //std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2(INPUT_FILE);;
    std::cout << "Part Two: " << part2 << std::endl;

    //assert(part1 == 33975);
    //assert(part2 == 29083);
    return 0;
}

/*

1600
12
1000
900
4
14

 
*/
