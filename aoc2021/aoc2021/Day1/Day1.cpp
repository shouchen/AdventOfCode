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

auto do_part(int window)
{
    auto count = 0;
    for (auto i = 0U; i < data.size() - window; i++)
        if (data[i + window] > data[i])
            count++;

    return count;
}

int main()
{
    read_data("input.txt");

    auto part1 = do_part(1);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part(3);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 1681);
    assert(part2 == 1704);
    return 0;
}
