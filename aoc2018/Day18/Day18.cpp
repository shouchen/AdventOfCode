#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

class Grid
{
public:
    unsigned generate();

private:
    void get_adjacent_counts(int row, int col, unsigned &trees, unsigned &lumberyards);
        
    std::vector<std::string> data;
    friend std::istream &operator>>(std::istream &istream, Grid &grid);
};

unsigned Grid::generate()
{
    std::vector<char *> mutate;
    unsigned trees = 0U, lumberyards = 0U;

    for (auto row = 0; row < data.size(); row++)
        for (auto col = 0; col < data[row].size(); col++)
        {
            unsigned tree_count = 0, lumberyard_count = 0;
            get_adjacent_counts(row, col, tree_count, lumberyard_count);

            switch (data[row][col])
            {
            case '.':
                if (tree_count >= 3)
                    mutate.push_back(&data[row][col]);
                break;
            case '|':
                if (lumberyard_count >= 3)
                    mutate.push_back(&data[row][col]);
                else
                    trees++;
                break;
            case '#':
                if (!lumberyard_count || !tree_count)
                    mutate.push_back(&data[row][col]);
                else
                    lumberyards++;
                break;
            }
        }

    for (auto c : mutate)
    {
        switch (*c)
        {
        case '.':
            *c = '|';
            trees++;
            break;
        case '|':
            *c = '#';
            lumberyards++;
            break;
        case '#':
            *c = '.';
            break;
        }
    }

    return trees * lumberyards;
}

void Grid::get_adjacent_counts(int row, int col, unsigned &trees, unsigned &lumberyards)
{
    trees = lumberyards = 0;

    for (auto i = -1; i <= 1; i++)
    {
        for (auto j = -1; j <= 1; j++)
        {
            if (i || j)
            {
                auto adj_row = row + i, adj_col = col + j;
                if (adj_row >= 0 && adj_row <= data.size() - 1 && adj_col >= 0 && adj_col <= data[row].size() - 1)
                {
                    auto c = data[adj_row][adj_col];
                    if (c == '|')
                        trees++;
                    else if (c == '#')
                        lumberyards++;
                }
            }
        }
    }
}

std::istream &operator>>(std::istream &istream, Grid &grid)
{
    grid.data.clear();
    std::string line;

    while (istream >> line)
        grid.data.push_back(line);
    return istream;
}

int main()
{
    Grid grid;
    std::ifstream file("input.txt");
    file >> grid;

    for (auto generation = 1; generation <= 16200; generation++)
    {
        auto score = grid.generate();

        if (generation == 10)
        {
            std::cout << "Part 1: " << score << std::endl << std::endl;
            assert(score == 394420);
        }
        else if (generation >= 16000)
            std::cout << "After " << generation << " generations = " << score << std::endl;
    }

    // For Part 2, observe a sample after some time and see if it's increasing
    // linearly, decreasing linearly, or is cyclic. Data shows that it's a cycle
    // of period 28. 1,000,000,000 mod 28 is 20, so we can extrapolate to that
    // future generation's score by taking any one of these later generations
    // where generation mod 28 = 20. For example, we can use
    //
    // After 16008 generations = 174420
    //
    // to infer that the answer to Part 2 must be 174420.

    return 0;
}
