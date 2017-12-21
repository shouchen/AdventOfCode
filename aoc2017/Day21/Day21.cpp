#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

std::map<std::string, std::string> rulebook;

auto do_input_transform(const std::string &input, const int *remapping)
{
    std::string output;

    for (auto i = 0; i < input.length(); i++)
        output.push_back(input[remapping[i]]);

    return output;
}

void add_rule_for_all_input_orientations(std::string input, std::string &output)
{
    static const int rotate4[] = { 2, 0, 3, 1 }, rotate9[] = { 6, 3, 0, 7, 4, 1, 8, 5, 2 };
    static const int flip_horizontal4[] = { 2, 3, 0, 1 }, flip_horizontal9[] = { 6, 7, 8, 3, 4, 5, 0, 1, 2 };
    static const int flip_vertical4[] = { 1, 0, 3, 2 }, flip_vertical9[] = { 2, 1, 0, 5, 4, 3, 8, 7, 6 };

    if (rulebook.find(input) == rulebook.end())
    {
        rulebook[input] = output;
        auto is2x2 = input.length() == 4;
        add_rule_for_all_input_orientations(do_input_transform(input, is2x2 ? rotate4 : rotate9), output);
        add_rule_for_all_input_orientations(do_input_transform(input, is2x2 ? flip_horizontal4 : flip_horizontal9), output);
        add_rule_for_all_input_orientations(do_input_transform(input, is2x2 ? flip_vertical4 : flip_vertical9), output);
    }
}

void populate_rules(const std::string &filename)
{
    rulebook.clear();

    std::ifstream f(filename);
    std::string input, arrow, output;

    while (f >> input >> arrow >> output)
    {
        input.erase(std::remove(input.begin(), input.end(), '/'), input.end());
        output.erase(std::remove(output.begin(), output.end(), '/'), output.end());
        add_rule_for_all_input_orientations(input, output);
    }
}

void do_iteration(std::string &image)
{
    auto input_size = unsigned(sqrt(image.length()));
    auto input_subsquare_size = (input_size % 2 == 0) ? 2U : 3U;
    auto output_subsquare_size = input_subsquare_size + 1;
    auto output_size = input_size / input_subsquare_size * output_subsquare_size;

    std::string next_image(output_size * output_size, ' ');

    for (auto row = 0U; row < input_size; row += input_subsquare_size)
    {
        for (auto col = 0U; col < input_size; col += input_subsquare_size)
        {
            std::string original;
            for (auto i = 0U; i < input_subsquare_size; i++)
                for (auto j = 0U; j < input_subsquare_size; j++)
                    original.push_back(image[(row + i) * input_size + (col + j)]);
            auto transformed = rulebook[original];

            auto start_out_row = row / input_subsquare_size * output_subsquare_size * output_size;
            auto start_out_col = col / input_subsquare_size * output_subsquare_size;
            const char *curr = transformed.c_str();

            for (auto i = 0U; i < output_subsquare_size; i++)
                for (auto j = 0U; j < output_subsquare_size; j++)
                {
                    auto index = start_out_row + i * output_size + start_out_col + j;
                    next_image[index] = *curr++;
                }
        }
    }

    image = next_image;
}

auto process_input(const std::string &filename, unsigned iterations)
{
    populate_rules(filename);

    std::string image = ".#...####";
    while (iterations--)
        do_iteration(image);

    return std::count(image.begin(), image.end(), '#');
}

int main()
{
    assert(process_input("input-test.txt", 2) == 12);

    auto part1 = process_input("input.txt", 5);
    auto part2 = process_input("input.txt", 18);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 117);
    assert(part2 == 2026963);
    return 0;
}
