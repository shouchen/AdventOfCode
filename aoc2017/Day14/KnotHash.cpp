#include "stdafx.h"
#include "KnotHash.h"
#include <vector>
#include <iomanip>
#include <sstream>

std::vector<int> initialize_data()
{
    std::vector<int> data;
    for (int i = 0; i < 256; i++)
        data.push_back(i);

    return data;
}

std::vector<int> make_input_binary(const std::string &input)
{
    std::vector<int> inputs;
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

std::string do_knot_hash(const std::string &input)
{
    auto data = initialize_data();
    auto inputs = make_input_binary(input);

    add_salt(inputs);

    do_rounds(data, inputs, 64);

    auto dense_hash = compress_hash(data);
    return hexify(dense_hash);
}
