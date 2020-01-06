#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <cmath>
#include <cassert>

std::map<std::string, std::map<std::string, int>> ingredients;
std::map<std::string, int> production_granularity;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);

    auto n1 = 0, n2 = 0;
    std::string s1, s2, arrow;
    std::vector<std::pair<int, std::string>> inputs;

    while (file >> n1 >> s1)
    {
        if (s1.back() == ',')
        {
            s1.pop_back();
            inputs.push_back(std::make_pair(n1, s1));
            continue;
        }

        inputs.push_back(std::make_pair(n1, s1));

        file >> arrow >> n2 >> s2;

        production_granularity[s2] = n2;
        for (auto input : inputs)
            ingredients[s2][input.second] = input.first;

        inputs.erase(inputs.begin(), inputs.end());
    }
}

std::map<std::string, long long> leftovers;

auto get_cost(long long n, const std::string &s)
{
    if (s == "ORE")
        return n;

    auto count = std::min(leftovers[s], n);
    n -= count;
    leftovers[s] -= count;

    auto production_factor = long long(ceil(double(n) / production_granularity[s]));

    auto cost = 0LL;
    for (auto i : ingredients[s])
        cost += get_cost(i.second * production_factor, i.first);

    leftovers[s] += production_granularity[s] * production_factor - n;
    return cost;
}

auto do_part1()
{
    return get_cost(1, "FUEL");
}

auto do_part2()
{
    const auto trillion = 1000000000000;
    auto high = 2LL;

    for (;;)
    {
        leftovers.clear();
        auto cost = get_cost(high, "FUEL");
        if (cost > trillion)
            break;
        high *= 2;
    }

    auto low = 1LL;
    for (;;)
    {
        leftovers.clear();
        auto middle = (low + high) / 2, cost = get_cost(middle, "FUEL");

        if (cost > trillion)
            high = middle;
        else
            low = middle;

        if (low >= high - 1)
            return low;
    }
}

int main() {
    read_input("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 387001);
    assert(part2 == 3412429);
    return 0;
}
