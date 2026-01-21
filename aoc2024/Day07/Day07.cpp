#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>

using ULL = unsigned long long;

auto concat(ULL a, ULL b)
{
    auto multiplier = 1ULL;
    while (b >= multiplier)
        multiplier *= 10;
    return a * multiplier + b;
}

auto could_possibly(const std::vector<ULL> &numbers, int start_index, ULL so_far, ULL target, bool allow_concat)
{
    if (start_index == numbers.size())
        return so_far == target;

    return
        (so_far <= target) &&
        could_possibly(numbers, start_index + 1, so_far * numbers[start_index], target, allow_concat) ||
        could_possibly(numbers, start_index + 1, so_far + numbers[start_index], target, allow_concat) ||
        (allow_concat && could_possibly(numbers, start_index + 1, concat(so_far, numbers[start_index]), target, allow_concat));
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::pair<ULL, ULL> retval;
    auto test_value = 0ULL, n = 0ULL;
    auto colon = ':';

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        ss >> test_value >> colon;

        std::vector<ULL> numbers;
        while (ss >> n)
            numbers.push_back(n);

        if (could_possibly(numbers, 1, numbers[0], test_value, false))
            retval.first += test_value;
        if (could_possibly(numbers, 1, numbers[0], test_value, true))
            retval.second += test_value;
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 4364915411363);
    assert(answer.second == 38322057216320);
    return 0;
}
