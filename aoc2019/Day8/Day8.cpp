#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

using Layer = std::vector<std::vector<unsigned>>;
std::vector<Layer> image;
std::vector<std::vector<char>> composite_image;

auto process_input_and_return_part1(unsigned rows, unsigned cols)
{
    std::ifstream file("input.txt");
    auto retval = 0U, minimum = UINT_MAX;

    for (auto i = 0U; i < rows; i++)
        composite_image.push_back(std::vector<char>(cols, '?'));

    for (;;)
    {
        Layer layer;
        std::vector<unsigned> count(3, 0);

        for (auto i = 0U; i < rows; i++)
        {
            std::vector<unsigned> row;
            for (auto j = 0U; j < cols; j++)
            {
                auto c = ' ';
                if (!(file >> c))
                    return retval;

                auto d = c - '0';
                row.push_back(d);
                count[d]++;

                auto &pixel = composite_image[i][j];
                if (pixel == '?')
                {
                    if (d == 0) pixel = ' ';
                    else if (d == 1) pixel = '*';
                }
            }
            layer.push_back(row);
        }
        image.push_back(layer);

        if (count[0] < minimum)
        {
            minimum = count[0];
            retval = count[1] * count[2];
        }
    }
}

std::ostream &part2(std::ostream &o)
{
    for (const auto &row : composite_image)
    {
        for (const auto &pixel : row) o << pixel;
        o << std::endl;
    }
    return o;
}

int main()
{
    auto part1 = process_input_and_return_part1(6, 25);
    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two:" << std::endl << part2;

    assert(part1 == 2684);
    // Part2 visually spells out "YGRYZ" in ASCII art
    return 0;
}
