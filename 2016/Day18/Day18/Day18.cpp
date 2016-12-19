#include "stdafx.h"
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <ctime>
#include <cassert>

inline void FindNext(std::string &input)
{
    std::string next;

    for (auto i = 0U; i < input.length(); i++)
    {
        char left = (i > 0) ? input[i - 1] : '.';
        char center = input[i];
        char right = (i < input.length() - 1) ? input[i + 1] : '.';

        next.push_back((left == right) ? '.' : '^');
    }

    input = next;
}

bool IsSafe(char c)
{
    return c == '.';
}

void DoProblem(std::string input, unsigned &afterForty, unsigned &afterFourHundredThousand)
{
    auto count = 0U;

    for (auto i = 0U; i < 400000; i++)
    {
        count += std::count_if(input.begin(), input.end(), IsSafe);

        if (i == 39)
            afterForty = count;

        FindNext(input);
    }

    afterFourHundredThousand = count;
}

int main()
{
    double startTime = clock();

    const std::string input = ".^^^^^.^^^..^^^^^...^.^..^^^.^^....^.^...^^^...^^^^..^...^...^^.^.^.......^..^^...^.^.^^..^^^^^...^.";
    auto afterForty = 0U, afterFourHundredThousand = 0U;

    DoProblem(input, afterForty, afterFourHundredThousand);

    std::cout << "Part One: " << afterForty << std::endl;
    std::cout << "Part Two: " << afterFourHundredThousand << std::endl;

    assert(afterForty == 1956);
    assert(afterFourHundredThousand == 19995121);

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;
    return 0;
}
