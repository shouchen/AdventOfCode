#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<int> list1, list2;
    auto n1 = 0, n2 = 0;

    while (file >> n1 >> n2)
    {
        list1.push_back(n1);
        list2.push_back(n2);
    }

    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    auto total = 0LL;

    for (auto i = 0; i < list1.size(); i++)
    {
        auto dist = std::abs(list1[i] - list2[i]);
        total += dist;
    }

    return total;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<int> left;
    std::map<int, int> right;
    auto n1 = 0, n2 = 0;

    while (file >> n1 >> n2)
    {
        left.push_back(n1);
        right[n2]++;
    }

    auto total = 0;

    for (auto i : left)
        total += i * right[i];

    return total;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 936063);
    assert(part2 == 23150395);
    return 0;
}
