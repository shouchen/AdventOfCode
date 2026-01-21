#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>

auto could_possibly(std::vector<unsigned long long> &numbers, int start_index, unsigned long long so_far, unsigned long long target)
{
    if (start_index == numbers.size())
        return so_far == target;

    return
        (so_far <= target) &&
        could_possibly(numbers, start_index + 1, so_far * numbers[start_index], target) ||
        could_possibly(numbers, start_index + 1, so_far + numbers[start_index], target);
}

auto concat(unsigned long long a, unsigned long long b)
{
    auto multiplier = 1ULL;
    while (b >= multiplier)
        multiplier *= 10;
    return a * multiplier + b;
}

auto could_possibly2(std::vector<unsigned long long> &numbers, int start_index, unsigned long long so_far, unsigned long long target)
{
    if (start_index == numbers.size())
        return so_far == target;

    return
        (so_far <= target) &&
        could_possibly2(numbers, start_index + 1, so_far * numbers[start_index], target) ||
        could_possibly2(numbers, start_index + 1, so_far + numbers[start_index], target) ||
        could_possibly2(numbers, start_index + 1, concat(so_far, numbers[start_index]), target);
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto colon = ':';
    auto retval = 0ULL;

    while (std::getline(file, line))
    {
        auto test_value = 0ULL;

        std::stringstream ss(line);
        ss >> test_value >> colon;

        auto n = 0ULL;
        std::vector<unsigned long long> numbers;
        while (ss >> n)
            numbers.push_back(n);

        if (could_possibly(numbers, 1, numbers[0], test_value))
            retval += test_value;
    }

    return retval;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto colon = ':';
    auto retval = 0ULL;

    while (std::getline(file, line))
    {
        auto test_value = 0ULL;

        std::stringstream ss(line);
        ss >> test_value >> colon;

        auto n = 0ULL;
        std::vector<unsigned long long> numbers;
        while (ss >> n)
            numbers.push_back(n);

        if (could_possibly2(numbers, 1, numbers[0], test_value))
        {
            std::cout << "Found match for " << line << std::endl;
            retval += test_value;
            std::cout << "  retval = " << retval << std::endl;
        }
    }

    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 4364915411363);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 38322057216320);

    return 0;
}
