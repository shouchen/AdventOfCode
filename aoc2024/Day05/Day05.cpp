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
        auto lesser = rule.first, greater = rule.second;

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

    while (std::getline(file, line) && !line.empty())
    {
        auto lesser = atoi(line.c_str());
        auto greater = atoi(line.c_str() + line.find('|') + 1);
        rules.push_back({ lesser, greater });
    }

    auto n = 0, retval = 0;
    auto comma = ',';

    while (std::getline(file, line))
    {
        line.push_back(comma);

        std::stringstream ss(line);
        std::vector<int> numbers;

        while (ss >> n >> comma)
            numbers.push_back(n);

        if (in_order(numbers))
            retval += numbers[numbers.size() / 2];
    }

    return retval;
}

auto sort_numbers(std::vector<int> &numbers)
{
    std::sort(numbers.begin(), numbers.end(), [](int a, int b) {
        for (auto &rule : rules)
            if (rule.first == a && rule.second == b)
                return true;
            else if (rule.first == b && rule.second == a)
                return false;

        assert(false);
        return false;
    });
}

auto do_part2(const std::string &filename)
{
    rules.clear();

    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line) && !line.empty())
    {
        auto lesser = atoi(line.c_str());
        auto greater = atoi(line.c_str() + line.find('|') + 1);
        rules.push_back({ lesser, greater });
    }

    auto n = 0, retval = 0;
    auto comma = ',';

    while (std::getline(file, line))
    {
        line.push_back(comma);

        std::stringstream ss(line);
        std::vector<int> numbers;

        while (ss >> n >> comma)
            numbers.push_back(n);

        if (!in_order(numbers))
        {
            sort_numbers(numbers);
            retval += numbers[numbers.size() / 2];
        }
    }

    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    //assert(part1 == 6498);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
