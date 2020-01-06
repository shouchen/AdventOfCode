#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

auto read_input(const std::string &filename, unsigned times)
{
    std::ifstream file(filename);
    std::string s;
    file >> s;

    std::vector<uint8_t> v;
    for (auto c : s)
        v.push_back(c - '0');

    if (times > 1)
    {
        // Figure out how many extra times we really need to replicate input data
        auto offset = atoi(s.substr(0, 7).c_str());
        const auto n = v.size(), repeats = times - offset / n;

        // Quickly clone the input data that many times
        v.resize(n * repeats);
        for (auto i = 1U; i < repeats; i++)
            std::memcpy(v.data() + i * n, v.data(), n * sizeof(v[0]));

        // Further trim input data to only the part that matters for our problem
        v.erase(v.begin(), v.begin() + offset % n);
    }

    return v;
}

void do_fft1(std::vector<uint8_t> &v)
{
    static const int pattern[] = { 0, 1, 0, -1 };
    std::vector<uint8_t> retval;
    retval.reserve(v.size());
    
    for (auto i = 0U; i < v.size(); i++)
    {
        auto total = 0, pattern_index = i ? 0 : 1, sub_repeat = i ? 1 : 0;

        for (auto n : v)
        {
            total += n * pattern[pattern_index];

            if (++sub_repeat == i + 1)
            {
                sub_repeat = 0;
                if (++pattern_index == 4)
                    pattern_index = 0;
            }
        }

        retval.push_back(std::abs(total) % 10);
    }

    v = retval;
}

void do_fft2(std::vector<uint8_t> &v)
{
    std::vector<uint8_t> retval(v.size(), 0);

    auto sum = 0;
    for (int i = v.size() - 1; i >= 0; --i)
    {
        sum += v[i];
        retval[i] = sum % 10;
    }

    v = retval;
}

auto do_part(const std::string &filename, int times, void fft(std::vector<uint8_t> &))
{
    auto v = read_input(filename, times);

    for (auto i = 0; i < 100; i++)
        fft(v);

    auto retval = 0;
    for (auto i = 0; i < 8; i++)
        retval = 10 * retval + v[i];

    return retval;
}

int main()
{
    auto part1 = do_part("input.txt", 1, do_fft1);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part("input.txt", 10000, do_fft2);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 27831665);
    assert(part2 == 36265589);
    return 0;
}
