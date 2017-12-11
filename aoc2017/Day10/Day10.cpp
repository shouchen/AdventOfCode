#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

std::vector<int> initialize_data(int data_size)
{
    std::vector<int> data;
    for (int i = 0; i < data_size; i++)
        data.push_back(i);

    return data;
}

std::vector<int> read_input_text(const std::string &filename)
{
    std::vector<int> inputs;
    std::ifstream f(filename);
    std::string input;

    while (std::getline(f, input, ','))
        inputs.push_back(atoi(input.c_str()));

    return inputs;
}

std::vector<int> read_input_binary(const std::string &filename)
{
    std::vector<int> inputs;
    std::string input;
    std::ifstream f(filename);

    f >> input;

    for (auto c : input)
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

int do_part1(int data_size, const std::string &filename)
{
    auto data = initialize_data(data_size);
    auto inputs = read_input_text(filename);

    do_rounds(data, inputs, 1);
    return data[0] * data[1];
}

std::string do_part2(int data_size, const std::string &filename)
{
    auto data = initialize_data(data_size);
    auto inputs = read_input_binary(filename);

    add_salt(inputs);

    do_rounds(data, inputs, 64);

    auto dense_hash = compress_hash(data);
    return hexify(dense_hash);
}

int main()
{
    assert(do_part1(5, "input-test.txt") == 12);

    auto part1 = do_part1(256, "input.txt");
    auto part2 = do_part2(256, "input.txt");

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 4480);
    assert(part2 == "c500ffe015c83b60fad2e4b7d59dabc4");
    return 0;
}
