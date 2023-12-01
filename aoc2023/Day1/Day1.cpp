#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

const std::vector<std::vector<std::string>> numbers =
{
    { "0" },         { "1", "one"},  { "2", "two"},    { "3", "three"},  { "4", "four" },
    { "5", "five" }, { "6", "six" }, { "7", "seven" }, { "8", "eight" }, { "9", "nine" }
};

auto find_number(const std::string &line, int dir, bool allow_spelling)
{
    for (auto i = (dir == 1) ? 0 : line.length() - 1; ; i += dir)
        for (auto j = 0; j < numbers.size(); j++)
            for (auto &k : numbers[j])
                if (line.substr(i, k.length()) == k)
                    return j;
                else if (!allow_spelling)
                    break;
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto retval = std::make_pair(0, 0);

    while (std::getline(file, line))
    {
        retval.first += 10 * find_number(line, 1, false) + find_number(line, -1, false);
        retval.second += 10 * find_number(line, 1, true) + find_number(line, -1, true);
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 53334);
    assert(answer.second == 52834);
    return 0;
}
