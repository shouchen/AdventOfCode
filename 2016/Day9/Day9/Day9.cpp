#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

unsigned long long compute_length(const std::string &line, bool recurse)
{
    auto count = 0ULL;

    for (auto i = 0U; i < line.length(); i++)
    {
        if (line[i] == '(')
        {
            auto x = line.find('x', ++i);
            auto repeat_length = atoi(line.substr(i, x).c_str());

            i = x + 1;
            auto parenth = line.find(')', i);
            auto b = atoi(line.substr(i, parenth).c_str());

            i = parenth + 1;
            count += b * (recurse ? compute_length(line.substr(i, repeat_length), true) : repeat_length);

            i += repeat_length - 1;
        }
        else
        {
            count++;
        }
    }

    return count;
}

int main()
{
    std::ifstream f("input.txt");
    std::string line;

    f >> line;

    auto answer1 = compute_length(line, false);
    auto answer2 = compute_length(line, true);

    std::cout << "Part One: " << answer1 << std::endl;
    std::cout << "Part Two: " << answer2 << std::endl;

    assert(answer1 == 70186);
    assert(answer2 == 10915059201);
    return 0;
}
