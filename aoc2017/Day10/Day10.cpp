#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

std::vector<int> data;
std::vector<int> inputs;

void reverse(int start, int len)
{
    int left = start, right = (start + len - 1) % data.size();
    int count = 0;

    do
    {
        auto temp = data[left];
        data[left] = data[right];
        data[right] = temp;
        if (++left >= data.size()) left = 0;
        if (--right < 0) right = data.size() - 1;
    } while (++count < len / 2);
}

void dump_data()
{
    for (int i = 0; i < data.size(); i++)
        std::cout << data[i];
    std::cout << std::endl;
}

void add_salt(std::vector<int> &data)
{
    data.push_back(17);
    data.push_back(31);
    data.push_back(73);
    data.push_back(47);
    data.push_back(23);
}

void compress_hash(const std::vector<int> &sparse_hash, std::vector<int> &dense_hash)
{
    for (int i = 0; i < 16; i++)
        dense_hash.push_back(0);

    for (int i = 0; i < 256; i++)
        dense_hash[i / 16] ^= data[i];
}

std::string hexify(const std::vector<int> data)
{
    std::stringstream ss;
    for (auto item : data)
        ss << std::setfill('0') << std::setw(2) << std::hex << item;

    return ss.str();
}

int do_part1(int data_size, std::vector<int> inputs)
{
    data.clear();
    for (int i = 0; i < data_size; i++)
        data.push_back(i);
    dump_data();

    auto curr = 0;
    auto skip_size = 0;
    for (auto input : inputs)
    {
        reverse(curr, input);
        dump_data();
        curr = (curr + input + skip_size++) % data.size();
    }

    return data[0] * data[1];
}

std::string do_part2(const std::string &input2)
{
    data.clear();
    for (int i = 0; i < 256; i++)
        data.push_back(i);

    dump_data();

    inputs.clear();
    for (auto c : input2)
        inputs.push_back(int(c));

    add_salt(inputs);

    auto curr = 0;
    auto skip_size = 0;

    for (int i = 0; i < 64; i++)
    {
        for (auto input : inputs)
        {
            reverse(curr, input);
            curr = (curr + input + skip_size++) % data.size();
        }
    }

    std::vector<int> dense_hash;
    compress_hash(data, dense_hash);

    return hexify(dense_hash);
}

int main()
{
    auto part1 = do_part1(5, { 3, 4, 1, 5 });
    assert(part1 == 12);

    part1 = do_part1(256, { 63, 144, 180, 149, 1, 255, 167, 84, 125, 65, 188, 0, 2, 254, 229, 24 });
    std::cout << "Part 1: " << part1 << std::endl;
    assert(part1 == 4480);

    auto part2 = do_part2("63,144,180,149,1,255,167,84,125,65,188,0,2,254,229,24");
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 4480);
    assert(part2 == "c500ffe015c83b60fad2e4b7d59dabc4");
    return 0;
}
