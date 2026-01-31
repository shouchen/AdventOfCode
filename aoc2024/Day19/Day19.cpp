#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <cassert>

using Patterns = std::vector<std::string>;

auto how_many_ways(const Patterns &patterns, const std::string &design)
{
    static std::map<std::string, unsigned long long> memo;

    if (design.empty())
        return 1ULL;

    if (memo.find(design) != memo.end())
        return memo[design];

    auto retval = 0ULL;

    for (const auto &p : patterns)
        if (design.length() >= p.length() && std::equal(p.begin(), p.end(), design.begin()))
            retval += how_many_ways(patterns, design.substr(p.length()));

    return memo[design] = retval;
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, pattern, design;
    auto comma = ',';

    std::getline(file, line);
    std::istringstream iss(line);

    Patterns patterns;
    while (iss >> pattern)
    {
        if (pattern.back() == ',') pattern.pop_back();
        patterns.push_back(pattern);
    }

    std::pair<int, unsigned long long> retval;
    while (file >> design)
    {
        auto how_many = how_many_ways(patterns, design);
        if (how_many) retval.first++;
        retval.second += how_many;
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 213);
    assert(answer.second == 1016700771200474);
    return 0;
}
