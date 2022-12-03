#include <iostream>
#include <fstream>
#include <cassert>

auto get_priority(char c)
{
    return c - ((c > 'Z') ? 96 : 38);
}

auto find_shared(const std::string &s1, const std::string &s2)
{
    for (auto c : s1)
        if (s2.find(c) != std::string::npos)
            return c;

    return ' ';
}

auto find_shared(const std::string &s1, const std::string &s2, const std::string &s3)
{
    for (auto c : s1)
        if (s2.find(c) != std::string::npos && s3.find(c) != std::string::npos)
            return c;

    return ' ';
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto retval = 0;

    while (file >> line)
    {
        auto mid = line.length() / 2;
        auto shared = find_shared(line.substr(0, mid), line.substr(mid));
        retval += get_priority(shared);
    }

    return retval;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string first, second, third;
    auto retval = 0;

    while (file >> first >> second >> third)
    {
        auto shared = find_shared(first, second, third);
        retval += get_priority(shared);
    }

    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 8298);
    assert(part2 == 2708);
    return 0;
}