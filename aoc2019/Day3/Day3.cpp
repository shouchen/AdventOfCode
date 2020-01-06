#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <cassert>

auto do_both_parts(const std::string &filename)
{
    static const std::pair<int, int> dirs[]{ { 0, -1 },{ 0, 1 },{ -1, 0 },{ 1, 0 } };
    std::map<std::pair<int, int>, int> dist1;
    auto retval = std::make_pair(INT32_MAX, INT32_MAX);

    std::ifstream file(filename);
    for (auto string = 1; string <= 2; string++)
    {
        std::string s;
        file >> s;

        std::stringstream ss(s);
        auto pos = std::make_pair(0, 0);
        auto step = 0;

        while (!ss.eof())
        {
            auto udlr = ' ', comma = ',';
            auto count = 0;
            ss >> udlr >> count >> comma;

            auto dir = dirs[std::string("UDLR").find(udlr)];

            for (auto i = 0; i < count; i++)
            {
                pos.first += dir.first, pos.second += dir.second, step++;

                if (string == 1)
                    dist1[pos] = step;
                else
                {
                    auto iter = dist1.find(pos);
                    if (iter != dist1.end())
                        retval = std::make_pair(
                            std::min(retval.first, std::abs(pos.first) + std::abs(pos.second)),
                            std::min(retval.second, iter->second + step));
                }
            }
        }
    }

    return retval;
}

int main()
{
    auto result = do_both_parts("input.txt");
    std::cout << "Part One: " << result.first << std::endl;
    std::cout << "Part Two: " << result.second << std::endl;

    assert(result.first == 721);
    assert(result.second == 7388);
    return 0;
}
