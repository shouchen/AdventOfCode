#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <numeric>
#include <cassert>

std::map <std::string, std::pair <std::string, std::string>> m;
std::string dirs;

auto find_cycle_len(std::string curr, bool part2)
{
    auto steps = 0LL;

    for (;;)
        for (auto d : dirs)
        {
            steps++;
            curr = (d == 'L') ? m[curr].first : m[curr].second;
            if (part2 ? curr.back() == 'Z' : curr == "ZZZ")
                return steps;
        }
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string a, equals, b, c;

    file >> dirs;

    while (file >> a >> equals >> b >> c)
        m[a] = std::make_pair(b.substr(1, 3), c.substr(0, 3));

    auto retval = std::make_pair(find_cycle_len("AAA", false), 1LL);

    for (auto &i : m)
        if (i.first.back() == 'A')
            retval.second = std::lcm(retval.second, find_cycle_len(i.first, true)); // C++17

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second  << std::endl;

    assert(answer.first == 11309);
    assert(answer.second == 13740108158591);
    return 0;
}