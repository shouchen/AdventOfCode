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
    std::vector<std::pair<int, int>> mutate;
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
                    mutate.push_back(std::make_pair(row, col));
                break;
            case '|':
                if (lumberyard_count >= 3)
                    mutate.push_back(std::make_pair(row, col));
                else
                    trees++;
                break;
            case '#':
                if (!lumberyard_count || !tree_count)
                    mutate.push_back(std::make_pair(row, col));
                else
                    lumberyards++;
                break;
            }
        }

    for (auto &change : mutate)
    {
        auto &c = data[change.first][change.second];
        switch (c)
        {
        case '.': c = '|'; trees++;  break;
        case '|': c = '#'; lumberyards++; break;
        case '#': c = '.'; break;
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

//void do_part1(Grid grid, unsigned n)
//{
//}
//
//void do_part2(Grid grid, unsigned n)
//{
//    Grid grid;
//    std::ifstream file("input.txt");
//    file >> grid;
//
//    for (auto i = 1U; i <= n; i++)
//    {
//        auto score = grid.generate();
//        std::cout << "After " << i << " times = " << score << std::endl;
//    }
//}

int main()
{
    Grid grid;
    std::ifstream file("input.txt");
    file >> grid;

    for (int generation = 1; generation <= 16200; generation++)
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

    // For Part 2, observe a sample after some time and see if increasing
    // linearly, decreasing linearly, or cyclic. Data shows that it's a cycle
    // of period 28. 1,000,000,000 mod 28 is 20, so we can extrapolate to that
    // future generation value by taking any other generation where mod 28 is
    // equal to 20. 
    //
    // Here, this would be 26410.

    //auto part1 = do_part1(grid, 10);
    //std::cout << "Part 1: " << part1 << std::endl;

    ////auto part2 = do_part2(10);
    ////std::cout << "Part 2: " << part2 << std::endl;

    //assert(part1 == 394420);
    ////assert(part2 == 26410);

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