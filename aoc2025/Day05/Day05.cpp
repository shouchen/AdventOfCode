#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>

std::vector<std::pair<unsigned long long, unsigned long long>> ranges;

auto is_fresh(unsigned long long ingredient)
{
    for (const auto &range : ranges)
        if (ingredient >= range.first && ingredient <= range.second)
            return true;
 
    return false;
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto retval = 0;

    while (std::getline(file, line) && line.length())
    {
        std::stringstream ss(line);
        auto from = 0ULL, to = 0ULL;
        auto dash = '-';
        
        ss >> from >> dash >> to;
        ranges.push_back(std::make_pair(from, to));
    }

    auto ingredient = 0ULL;
    while (file >> ingredient)
    {
        if (is_fresh(ingredient))
            retval++;
    }

    return retval;
}

int main()
{
    auto part1 = solve("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 611);

    //auto part2 = solve(true);
    //std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == 8409);

    return 0;
}
