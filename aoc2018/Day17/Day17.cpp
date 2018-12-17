#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <queue>
#include <list>
#include <stack>
#include <map>
#include <string>
#include <numeric>
#include <algorithm>
#include <cassert>


char grid[1820][700];
int min_depth = std::numeric_limits<int>::max();
int max_depth = std::numeric_limits<int>::min();
int min_x = std::numeric_limits<int>::max();
int max_x = std::numeric_limits<int>::min();

unsigned dump()
{
    unsigned count = 0;
    for (auto row = min_depth; row <= max_depth; row++)
    {
        for (auto col = min_x; col <= max_x; col++)
        {
            auto c = grid[row][col];
            std::cout << (c ? c : '.');
            if (c == '|' || c == '~') count++;
        }
        std::cout << std::endl;
    }

    std::string s;
    std::cout << count << " =============================================" << std::endl;
    std::getline(std::cin, s);
    return count;
}

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    char var1, equals = '=', var2, comma = ',', dot = '.';
    int dim1, dim2a, dim2b;

    grid[0][500] = '+';

    while (file >> var1 >> equals >> dim1 >> comma >> var2 >> equals >> dim2a >> dot >> dot >> dim2b)
    {
        if (var1 == 'x')
        {
            for (int row = dim2a; row <= dim2b; row++)
                grid[row][dim1] = '#';
            min_depth = std::min(min_depth, dim2a);
            max_depth = std::max(max_depth, dim2b);
            min_x = std::min(min_x, dim1);
            max_x = std::max(max_x, dim1);
        }
        else
        {
            for (int col = dim2a; col <= dim2b; col++)
                grid[dim1][col] = '#';
            min_depth = std::min(min_depth, dim1);
            max_depth = std::max(max_depth, dim1);
            min_x = std::min(min_x, dim2a);
            max_x = std::max(max_x, dim2b);
        }
    }
}

void drip(int row, int col)
{
    auto loc = std::make_pair(row, col);

    while (++loc.first <= max_depth)
    {
        if (!grid[loc.first][loc.second])
        {
            grid[loc.first][loc.second] = '|';
            dump();
        }
        else if (grid[loc.first][loc.second] == '#')
        {
            for (;;)
            {
                loc.first--;

                grid[loc.first][loc.second] = '|';
                dump();

                int left_wall = -1, right_wall = -1;

                // fill left
                auto left_loc = loc;

                for (;;)
                {
                    --left_loc.second;
                    if (!grid[left_loc.first][left_loc.second])
                    {
                        grid[left_loc.first][left_loc.second] = '|';
                        dump();

                        auto under = left_loc;
                        under.first++;

                        if (!grid[under.first][under.second])
                        {
                            drip(left_loc.first, left_loc.second);
                            break;
                        }
                    }
                    else if (grid[left_loc.first][left_loc.second] == '#')
                    {
                        left_wall = left_loc.second + 1;
                        break;
                    }
                }

                // fill right
                auto right_loc = loc;

                for (;;)
                {
                    right_loc.second++;
                    if (!grid[right_loc.first][right_loc.second])
                    {
                        grid[right_loc.first][right_loc.second] = '|';
                        dump();

                        auto under = right_loc;
                        under.first++;

                        if (!grid[under.first][under.second])
                        {
                            drip(right_loc.first, right_loc.second);
                            break;
                        }
                    }
                    else if (grid[right_loc.first][right_loc.second] == '#')
                    {
                        right_wall = right_loc.second - 1;
                        break;
                    }
                }

                // convert whole row into ~
                if (right_wall != -1 && left_wall != -1)
                {
                    for (auto col = left_wall; col <= right_wall; col++)
                        grid[loc.first][col] = '~';
                    dump();
                }
                else
                    return;
            }
        }
    }
}

void do_part1()
{
    drip(0, 500);
}

int main()
{
    read_input("input.txt");

    dump();
    do_part1();
    std::cout << dump() << std::endl;

    //assert(time == 10101);

    return 0;


}
// 146921 was too high