#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cassert>

std::vector<std::string> grid;

unsigned long long get_score()
{
    unsigned long long trees = 0, lumberyards = 0;
    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].size(); col++)
        {
            if (grid[row][col] == '|') trees++;
            if (grid[row][col] == '#') lumberyards++;
        }

    return trees * lumberyards;
}

void dump()
{
    for (auto row : grid)
        std::cout << row << std::endl;

    std::cout << "Score = " << get_score() << std::endl << std::endl;
}

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    while (file >> line)
        grid.push_back(line);
}

void apply_change()
{
    auto new_grid = grid;

    for (auto row = 0; row < new_grid.size(); row++)
        for (auto col = 0; col < new_grid[row].size(); col++)
        {
            std::vector<std::pair<int, int>> adj;
            adj.clear();
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++)
                {
                    if (i == 0 && j == 0) continue;

                    auto adj_row = row + i;
                    auto adj_col = col + j;
                    if (adj_row >= 0 && adj_row < new_grid.size() && adj_col >= 0 && adj_col < new_grid[adj_row].size())
                        adj.push_back(std::make_pair(adj_row, adj_col));
                }

            unsigned long long tree_count = 0, lumberyard_count = 0;
            for (auto a : adj)
            {
                if (grid[a.first][a.second] == '|') tree_count++;
                if (grid[a.first][a.second] == '#') lumberyard_count++;
            }

            if (grid[row][col] == '.')
            {
                if (tree_count >= 3)
                    new_grid[row][col] = '|';
            }
            else if (grid[row][col] == '|')
            {
                if (lumberyard_count >= 3)
                    new_grid[row][col] = '#';
            }
            else if (grid[row][col] == '#')
            {
                if (lumberyard_count >= 1 && tree_count >= 1)
                {
                }
                else
                    new_grid[row][col] = '.';
            }
        }
    grid = new_grid;
}

void apply_change_faster()
{
    std::vector<std::pair<int, int>> will_change;

    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].size(); col++)
        {
            std::vector<std::pair<int, int>> adj;
            adj.clear();
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++)
                {
                    if (i == 0 && j == 0) continue;

                    auto adj_row = row + i;
                    auto adj_col = col + j;
                    if (adj_row >= 0 && adj_row < grid.size() && adj_col >= 0 && adj_col < grid[adj_row].size())
                        adj.push_back(std::make_pair(adj_row, adj_col));
                }

            unsigned long long tree_count = 0, lumberyard_count = 0;
            for (auto a : adj)
            {
                if (grid[a.first][a.second] == '|') tree_count++;
                if (grid[a.first][a.second] == '#') lumberyard_count++;
            }

            if (grid[row][col] == '.')
            {
                if (tree_count >= 3)
                    will_change.push_back(std::make_pair(row, col));
                    //grid[row][col] = '|';
            }
            else if (grid[row][col] == '|')
            {
                if (lumberyard_count >= 3)
                    will_change.push_back(std::make_pair(row, col));
                    //grid[row][col] = '#';
            }
            else if (grid[row][col] == '#')
            {
                if (lumberyard_count >= 1 && tree_count >= 1)
                {
                }
                else
                {
                    will_change.push_back(std::make_pair(row, col));
                    //grid[row][col] = '.';
                }
            }
        }

    for (auto &change : will_change)
    {
        auto c = grid[change.first][change.second];
        if (c == '.') c = '|';
        else if (c == '|') c = '#';
        else c = '.';
        grid[change.first][change.second] = c;
    }
}

unsigned long long do_part1(unsigned n)
{
    grid.clear();
    read_input("input.txt");

    for (int i = 0; i < n; i++)
    {
        if (i % 1000000 == 0) std::cout << i << std::endl;
        apply_change_faster();
    }

    return get_score();
}

unsigned do_part2(unsigned n)
{
    grid.clear();
    read_input("input.txt");

    for (int i = 1; i <= n; i++)
    {
        apply_change_faster();
        std::cout << "After " << i << " times = " << get_score() << std::endl;
    }
}

int main()
{

    auto part1 = do_part1(10);
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = do_part2(10);
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 394420);
    //assert(part2 == 26410);

    return 0;
}

/*

Run the second part for long enough and it gets into a cyclic pattern:

After 15646 times = 182590    22         cycle = 28 (numbers to the left show iteration % 28)
After 15647 times = 189630    23
After 15648 times = 196452    24
After 15649 times = 205000    25
After 15650 times = 210789    26
After 15651 times = 218240    27
After 15652 times = 222824    0
After 15653 times = 231336    1
After 15654 times = 236155    2
After 15655 times = 240482    3
After 15656 times = 237636    4
After 15657 times = 237082    5
After 15658 times = 234938    6
After 15659 times = 237900    7
After 15660 times = 232232    8
After 15661 times = 228274    9
After 15662 times = 220584   10
After 15663 times = 216752   11
After 15664 times = 206702   12
After 15665 times = 199824   13
After 15666 times = 178100   14
After 15667 times = 175587   15
After 15668 times = 169880   16
After 15669 times = 173014   17
After 15670 times = 171248   18
After 15671 times = 174782   19
After 15672 times = 174420   20
After 15673 times = 179800   21
After 15674 times = 182590   22

So, 1 billion % 28 = 20 --> 174420 is the answer.

A better solution for part2 would be looking for these cycles and inferring it after some point.
But it's also possible that it's not cyclic but diverging, in which case, would also need to infer
that as well and find a linear growing or shrinking score and do that inference as well.

*/