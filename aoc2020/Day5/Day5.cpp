#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <array>
#include <algorithm>
#include <sstream>
#include <cassert>

auto split(std::stringstream &ss, int len)
{
    auto hi = (1 << len) - 1, lo = 0;
    char c;

    while (len--)
    {
        ss >> c;
        auto mid = (hi - lo) / 2 + lo;
        if (c == 'F' || c == 'L')
            hi = mid;
        else
            lo = mid + 1;
    }

    return hi;
}

auto process_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string s;
    std::set<int> ids;
    auto hi = 0;

    while (file >> s)
    {
        std::stringstream ss(s);
        auto id = split(ss, 7) * 8 + split(ss, 3);
        ids.insert(id);
        hi = std::max(hi, id);
    }

    for (auto i = 1; ; i++)
        if (ids.find(i) == ids.end() && ids.find(i - 1) != ids.end() && ids.find(i + 1) != ids.end())
            return std::make_pair(hi, i);
}

int main()
{
    auto result = process_input("input.txt");
    std::cout << "Part One: " << result.first << std::endl;
    std::cout << "Part Two: " << result.second << std::endl;

    assert(result.first == 871);
    assert(result.second == 640);
    return 0;
}
