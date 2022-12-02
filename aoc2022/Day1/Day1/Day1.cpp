#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <string>
#include <algorithm>
#include <numeric>

auto read_data(const std::string &filename)
{
    std::vector<int> data = { 0 };
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
            data.push_back(0);
        else
            data.back() += atoi(line.c_str());
    }

    std::sort(data.begin(), data.end());
    return data;
}

auto do_part(std::vector<int> &data, int n)
{
    return std::accumulate(data.end() - n, data.end(), 0);
}

int main()
{
    auto data = read_data("input.txt");

    auto part1 = do_part(data, 1);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part(data, 3);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 69693);
    assert(part2 == 200945);
    return 0;
}
