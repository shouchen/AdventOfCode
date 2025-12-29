#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

auto get_joltage(const std::string &bank, int n)
{
    auto startpos = bank.begin(), endpos = bank.end() - n;
    auto retval = std::string{};

    while (retval.length() < n)
    {
        endpos++;

        auto maxpos = std::max_element(startpos, endpos);
        retval.push_back(*maxpos);

        startpos = maxpos + 1;
    }

    return std::stoull(retval);
}

auto solve(const std::string &filename, int n)
{
    std::ifstream file(filename);
    std::string bank;
    auto retval = 0ULL;

    while (std::getline(file, bank))
        retval += get_joltage(bank, n);

    return retval;
}

int main()
{
    auto part1 = solve("input.txt", 2);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = solve("input.txt", 12);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 17343);
    assert(part2 == 172664333119298);
    return 0;
}
