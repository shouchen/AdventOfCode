#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

std::vector<std::pair<int,int>> rules;

auto in_order(const std::vector<int> &numbers)
{
    for (auto &rule : rules)
    {
        auto lesser = rule.first;
        auto greater = rule.second;

        if (std::find(numbers.begin(), numbers.end(), lesser) == numbers.end() ||
            std::find(numbers.begin(), numbers.end(), greater) == numbers.end())
            continue;

        if (std::find(numbers.begin(), numbers.end(), lesser) >
            std::find(numbers.begin(), numbers.end(), greater))
            return false;
    }

    return true;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto retval = 0;

    while (std::getline(file, line) && !line.empty())
    {
        auto lesser = atoi(line.c_str());
        auto greater = atoi(line.c_str() + line.find('|') + 1);
        rules.push_back({ lesser, greater });
    }

    auto comma = ',';
    while (std::getline(file, line))
    {
        line.push_back(comma);
        std::stringstream ss(line);
        auto n = 0;
        std::vector<int> numbers;

        while (ss >> n >> comma)
            numbers.push_back(n);

        if (in_order(numbers))
            retval += numbers[numbers.size() / 2];
    }

    return retval;
}

auto do_part2(const std::string &filename)
{
    auto retval = 0;
    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 6498);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
