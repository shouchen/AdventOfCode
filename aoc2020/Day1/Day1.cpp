#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

std::vector<int> data;

void read_data(const std::string &filename)
{
    std::ifstream file(filename);
    auto n = 0;

    while (file >> n)
        data.push_back(n);
}

int solve(int index, int sum, int n)
{
    for (auto i = index; i < data.size(); i++)
    {
        auto rest = (n == 1)
            ? ((data[i] == sum) ? 1 : 0)
            : solve(i + 1, sum - data[i], n - 1);

        if (rest)
            return data[i] * rest;
    }

    return 0;
}

int main()
{
    read_data("input.txt");

    auto part1 = solve(0, 2020, 2);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = solve(0, 2020, 3);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 381699);
    assert(part2 == 111605670);
    return 0;
}
