#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

auto get_max_joltage(const std::string &bank)
{
    auto first = '0', second = '0';
    auto savepos = 0;
    for (auto i = 0; i < bank.length() - 1; i++)
        if (bank[i] > first)
        {
            first = bank[i];
            savepos = i;
        }

    for (auto i = savepos + 1; i < bank.length(); i++)
        if (bank[i] > second)
            second = bank[i];

    return (first - '0') * 10 + (second - '0');
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string bank;
    auto retval = 0;

    while (std::getline(file, bank))
        retval += get_max_joltage(bank);

    return retval;
}

int main()
{
    auto part1 = solve("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 17343);
    
    // std::cout << "Part Two: " << solution.second << std::endl;
    // assert(solution.second == );
    return 0;
}
