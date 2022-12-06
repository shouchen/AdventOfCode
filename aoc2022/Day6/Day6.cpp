#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

auto any_repeats(const std::string &s, int start, int end)
{
    if (start >= end)
        return false;

    for (auto other = start + 1; other <= end; other++)
        if (s[start] == s[other])
            return true;

    return any_repeats(s, start + 1, end);
}

auto do_part(const std::string &filename, int n)
{
    std::ifstream file(filename);
    std::string line;
    file >> line;

    auto start = 0, end = n - 1;
    while (any_repeats(line, start, end))
        start++, end++;

    return end + 1;
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
