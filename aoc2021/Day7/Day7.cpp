#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

auto do_part(const std::string &filename, bool part2)
{
    std::vector<int> crabs;
    auto min = INT_MAX, max = INT_MIN;

    std::ifstream file(filename);
    auto n = 0;
    auto comma = ',';

    while (file >> n)
    {
        crabs.push_back(n);

        if (n < min) min = n;
        if (n > max) max = n;

        file >> comma;
    }

    auto min_cost = INT_MAX;
    for (auto i = min; i <= max; i++)
    {
        auto cost = 0;
        for (auto c : crabs)
        {
            auto diff = abs(c - i);
            cost += part2 ? (diff * (diff + 1) / 2) : diff;
        }

        if (cost < min_cost)
            min_cost = cost;
    }

    return min_cost;
}

int main()
{
    auto part1 = do_part("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 341558);
    assert(part2 == 93214037);
    return 0;
}
