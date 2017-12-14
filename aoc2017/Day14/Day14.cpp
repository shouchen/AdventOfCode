#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <string>
#include <iomanip>
#include <sstream>

std::vector<int> initialize_data()
{
    std::vector<int> data;
    for (int i = 0; i < 256; i++)
        data.push_back(i);

    return data;
}

std::vector<int> read_input_binary(const std::string &input, unsigned row)
{
    std::stringstream ss;
    ss << input << '-' << row;

    std::vector<int> inputs;
    for (auto c : ss.str())
        inputs.push_back(int(c));

    return inputs;
}

void add_salt(std::vector<int> &inputs)
{
    const static std::vector<int> salt = { 17, 31, 73, 47, 23 };
    for (auto i : salt)
        inputs.push_back(i);
}

void reverse(std::vector<int> &data, int start, int len)
{
    auto a = start, b = int((start + len - 1) % data.size());
    auto count = 0;

    do
    {
        std::swap(data[a], data[b]);

        if (++a >= data.size()) a = 0;
        if (--b < 0) b = int(data.size() - 1);
    } while (++count < len / 2);
}

void do_rounds(std::vector<int> &data, const std::vector<int> &inputs, int rounds)
{
    auto curr = 0, skip_size = 0;

    for (auto i = 0; i < rounds; i++)
    {
        for (auto input : inputs)
        {
            reverse(data, curr, input);
            curr = (curr + input + skip_size++) % data.size();
        }
    }
}

std::vector<int> compress_hash(const std::vector<int> &sparse_hash)
{
    std::vector<int> dense_hash;

    for (auto i = 0; i < 16; i++)
        dense_hash.push_back(0);

    for (auto i = 0; i < 256; i++)
        dense_hash[i / 16] ^= sparse_hash[i];

    return dense_hash;
}

std::string hexify(const std::vector<int> data)
{
    std::stringstream ss;
    for (auto item : data)
        ss << std::setfill('0') << std::setw(2) << std::hex << item;

    return ss.str();
}

std::string do_part2(const std::string &input, unsigned row)
{
    auto data = initialize_data();
    auto inputs = read_input_binary(input, row);

    add_salt(inputs);

    do_rounds(data, inputs, 64);

    auto dense_hash = compress_hash(data);
    return hexify(dense_hash);
}

bool grid[128][128];

void mark(unsigned row, unsigned col)
{
    grid[row][col] = false;

    if (row > 0 && grid[row - 1][col])
        mark(row - 1, col);
    if (row < 127 && grid[row + 1][col])
        mark(row + 1, col);
    if (col > 0 && grid[row][col - 1])
        mark(row, col - 1);
    if (col < 127 && grid[row][col + 1])
        mark(row, col + 1);
}

unsigned islandize()
{
    unsigned group_num = 0;
    for (int row = 0; row < 128; row++)
    {
        for (int col = 0; col < 128; col++)
        {
            if (grid[row][col])
            {
                group_num++;
                mark(row, col);
            }
        }
    }

    return group_num;
}

int main()
{
    //std::string input = "flqrgnkx"; // test
    std::string input = "vbqugkhl"; // real

    for (auto row = 0U; row < 128L; row++)
    {
        auto hash = do_part2(input, row);

        for (int i = 0; i < 32; i++)
        {
            unsigned val = (hash[i] >= 'a') ? unsigned(hash[i] - 'a' + 10) : unsigned(hash[i] - '0');
            grid[row][4 * i + 0] = (val & 0x8) == 0x8;
            grid[row][4 * i + 1] = (val & 0x4) == 0x4;
            grid[row][4 * i + 2] = (val & 0x2) == 0x2;
            grid[row][4 * i + 3] = (val & 0x1) == 0x1;
        }
    }

    auto part1 = 0U;
    for (auto row = 0U; row < 128; row++)
    {
        for (auto col = 0U; col < 128; col++)
        {
            if (grid[row][col])
                part1++;
        }
    }

    std::cout << "Part 1: " << part1 << std::endl;
    //assert(part1 == 8108); // test
    assert(part1 == 8148); // real

    for (auto row = 0; row < 128; row++)
    {
        for (auto col = 0; col < 128; col++)
            std::cout << (grid[row][col] ? '#' : '.');

        std::cout << std::endl;
    }
    std::cout << std::endl;

    auto part2 = islandize();
    std::cout << "Part 2: " << part2 << std::endl;
    //assert(part2 == 1242); // test
    assert(part2 == 1180); // real

    return 0;
}
