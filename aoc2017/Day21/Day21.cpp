#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

std::map<std::string, std::string> rulebook;

std::string rotate_cw(const std::string &input)
{
    std::string output;
    if (input.length() == 4)
    {
        output.push_back(input[2]);
        output.push_back(input[0]);
        output.push_back(input[3]);
        output.push_back(input[1]);
    }
    else
    {
        assert(input.length() == 9);
        output.push_back(input[6]);
        output.push_back(input[3]);
        output.push_back(input[0]);
        output.push_back(input[7]);
        output.push_back(input[4]);
        output.push_back(input[1]);
        output.push_back(input[8]);
        output.push_back(input[5]);
        output.push_back(input[2]);
    }
    return output;
}

std::string flip_horizontal(const std::string &input)
{
    std::string output;
    if (input.length() == 4)
    {
        output.push_back(input[2]);
        output.push_back(input[3]);
        output.push_back(input[0]);
        output.push_back(input[1]);
    }
    else
    {
        assert(input.length() == 9);
        output.push_back(input[6]);
        output.push_back(input[7]);
        output.push_back(input[8]);
        output.push_back(input[3]);
        output.push_back(input[4]);
        output.push_back(input[5]);
        output.push_back(input[0]);
        output.push_back(input[1]);
        output.push_back(input[2]);
    }
    return output;
}

std::string flip_vertical(const std::string &input)
{
    std::string output;
    if (input.length() == 4)
    {
        output.push_back(input[1]);
        output.push_back(input[0]);
        output.push_back(input[3]);
        output.push_back(input[2]);
    }
    else
    {
        assert(input.length() == 9);
        output.push_back(input[2]);
        output.push_back(input[1]);
        output.push_back(input[0]);
        output.push_back(input[5]);
        output.push_back(input[4]);
        output.push_back(input[3]);
        output.push_back(input[8]);
        output.push_back(input[7]);
        output.push_back(input[6]);
    }
    return output;
}

std::string apply_rule(const std::string &input)
{
    auto found = rulebook.find(input);
    assert(found != rulebook.end());
    return found->second;
}

void add_rule(std::string input, std::string &output)
{
    if (rulebook.find(input) != rulebook.end())
        return;

    rulebook[input] = output;
    add_rule(rotate_cw(input), output);
    add_rule(flip_horizontal(input), output);
    add_rule(flip_vertical(input), output);
}

unsigned count_on(const std::string &s)
{
    unsigned ret_val = 0;
    for (auto c : s)
        if (c == '#')
            ret_val++;
    return ret_val;
}

unsigned process_input(const std::string &filename)
{
}

int main()
{
    std::string image = ".#...####";

    std::ifstream f("input.txt");
    std::string line;
    std::vector<std::string> rules;
    while (getline(f, line))
    {
        char c;
        std::string input, output;
        auto input_size = line.find('/');
        auto output_size = input_size + 1;

        std::stringstream ss(line);

        for (auto row = 0; row < input_size; row++)
        {
            for (auto col = 0; col < input_size; col++)
            {
                ss >> c;
                assert(c == '#' || c == '.');
                input.push_back(c);
            }
            ss >> c;
        }
        ss >> c;

        for (auto row = 0; row < output_size; row++)
        {
            for (auto col = 0; col < output_size; col++)
            {
                ss >> c;
                assert(c == '#' || c == '.');
                output.push_back(c);
            }
            ss >> c;
        }

        add_rule(input, output);
    }

    // PART ONE
    for (auto iterations = 0; iterations < 5; iterations++)
    {
        auto size = unsigned(sqrt(image.length() + 0.1));
        if (size % 2 == 0)
        {
            auto output_size = size * 3 / 2;
            std::string next_image(output_size * output_size, ' ');
            for (auto row = 0; row < size; row += 2)
            {
                for (auto col = 0; col < size; col += 2)
                {
                    std::string input;
                    input.push_back(image[row * size + col]);
                    input.push_back(image[row * size + col + 1]);
                    input.push_back(image[(row + 1) * size + col]);
                    input.push_back(image[(row + 1) * size + col + 1]);
                    auto transform = apply_rule(input);
                    int start_out_row = row / 2 * 3 * output_size, start_out_col = col / 2 * 3;
                    const char *curr = transform.c_str();
                    for (int i = 0; i < 3; i++)
                        for (int j = 0; j < 3; j++)
                        {
                            auto index = start_out_row + i * output_size + start_out_col + j;
                            next_image[index] = *curr++;
                        }
                }
            }
            image = next_image;
        }
        else if (size % 3 == 0)
        {
            auto output_size = size * 4 / 3;
            std::string next_image(output_size * output_size, ' ');
            for (auto row = 0; row < size; row += 3)
            {
                for (auto col = 0; col < size; col +=3 )
                {
                    std::string input;
                    input.push_back(image[row * size + col]);
                    input.push_back(image[row * size + col + 1]);
                    input.push_back(image[row * size + col + 2]);
                    input.push_back(image[(row + 1) * size + col]);
                    input.push_back(image[(row + 1) * size + col + 1]);
                    input.push_back(image[(row + 1) * size + col + 2]);
                    input.push_back(image[(row + 2) * size + col]);
                    input.push_back(image[(row + 2) * size + col + 1]);
                    input.push_back(image[(row + 2) * size + col + 2]);
                    auto transform = apply_rule(input);
                    int start_out_row = row / 3 * 4 * output_size, start_out_col = col / 3 * 4;
                    const char *curr = transform.c_str();
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                        {
                            auto index = start_out_row + i * output_size + start_out_col + j;
                            next_image[index] = *curr++;
                        }
                }
            }
            image = next_image;
        }
    }

    auto part1 = count_on(image);
    std::cout << part1 << std::endl;
    //assert(part1 == 117);
    //assert(part2 == 2026963);
    return 0;
}
