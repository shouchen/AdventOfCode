#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

std::vector<std::pair<int,int>> rules;

auto compare(int lhs, int rhs)
{
    for (auto &rule : rules)
        if (rule.first == lhs && rule.second == rhs)
            return true;
        else if (rule.first == rhs && rule.second == lhs)
            return false;

    return false;
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line) && !line.empty())
    {
        auto lesser = atoi(line.c_str());
        auto greater = atoi(line.c_str() + line.find('|') + 1);
        rules.push_back({ lesser, greater });
    }

    std::pair<int, int> retval;
    auto comma = ',';    

    while (std::getline(file, line))
    {
        line.push_back(comma);

        std::stringstream ss(line);
        std::vector<int> numbers;
        auto n = 0;

        while (ss >> n >> comma)
            numbers.push_back(n);

        if (std::is_sorted(numbers.begin(), numbers.end(), compare))
        {
            retval.first += numbers[numbers.size() / 2];
        }
        else
        {
            std::sort(numbers.begin(), numbers.end(), compare);
            retval.second += numbers[numbers.size() / 2];
        }
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 6498);
    assert(answer.second == 5017);
    return 0;
}
