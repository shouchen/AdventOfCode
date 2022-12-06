#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

auto any_repeats(std::string::iterator begin, std::string::iterator end)
{
    if (begin >= end)
        return false;

    for (auto other = begin + 1; other < end; other++)
        if (*begin == *other)
            return true;

    return any_repeats(begin + 1, end);
}

auto do_part(const std::string &filename, int n)
{
    std::ifstream file(filename);
    std::string line;
    file >> line;

    auto begin = line.begin(), end = line.begin() + n;
    while (any_repeats(begin, end))
        begin++, end++;

    return end - line.begin();
}

int main()
{
    auto part1 = do_part("input.txt", 4);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part("input.txt", 14);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 1042);
    assert(part2 == 2980);
    return 0;
}
