#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <algorithm>
#include <cassert>

std::vector<long long> data;

void read_data(const std::string &filename)
{
    std::ifstream file(filename);
    auto n = 0LL;

    while (file >> n)
        data.push_back(n);
}

auto find_sum(int start, int end, int target, bool include_intermediates = false)
{
    for (auto i = start; i < end; i++)
    {
        auto sum = data[i];
        for (auto j = i + 1; j < end; j++)
        {
            sum = (include_intermediates ? sum : data[i]) + data[j];
            if (sum == target)
                return std::make_pair(i, j);
        }
    }

    return std::make_pair(-1, -1);
}

auto do_part1(int preamble)
{
    for (auto i = preamble; i < data.size(); i++)
        if (find_sum(i - preamble, i, data[i]).first == -1)
            return data[i];

    return -1LL;
}

auto do_part2(long long target)
{
    auto pair = find_sum(0, data.size() - 1, target, true);

    return
        *std::min_element(data.begin() + pair.first, data.begin() + pair.second + 1) +
        *std::max_element(data.begin() + pair.first, data.begin() + pair.second + 1);
}

int main()
{
    read_data("input.txt");

    auto part1 = do_part1(25);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2(part1);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 104054607);
    assert(part2 == 13935797);
    return 0;
}
