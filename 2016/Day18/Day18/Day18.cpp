#include "stdafx.h"
#include <iostream>
#include <string>
#include <cassert>

std::string FindNext(const std::string &input)
{
    std::string next;

    for (auto i = 0U; i < input.length(); i++)
    {
        char left = (i > 0) ? input[i - 1] : '.';
        char center = input[i];
        char right = (i < input.length() - 1) ? input[i + 1] : '.';

        next.push_back((left == right) ? '.' : '^');
    }

    return next;
}

unsigned count;

int main()
{
    std::string input = ".^^^^^.^^^..^^^^^...^.^..^^^.^^....^.^...^^^...^^^^..^...^...^^.^.^.......^..^^...^.^.^^..^^^^^...^.";

    for (auto i = 0U; i < 400000; i++)
    {
        for (auto c : input)
            if (c == '.')
                count++;

        input = FindNext(input);
    }

    std::cout << count << std::endl;

    //assert(count == 1956);
    //assert(count == 19995121);
    return 0;
}
