#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <ctime>
#include <cassert>

void DoProblem(std::string input, unsigned &afterForty, unsigned &afterFourHundredThousand)
{
    const unsigned length = input.length();
    std::string buffer(length, ' ');

    char *curr = &input[0], *other = &buffer[0];
    auto count = 0U;
    auto nextLineCount = std::count_if(curr, curr + length, [](char c) -> bool { return c == '.'; });

    for (auto i = 0U; i < 400000; i++)
    {
        count += nextLineCount;

        if (i == 39)
            afterForty = count;

        // Generate next line and count safes at the same time
        nextLineCount = 0L;

        for (auto j = 0U; j < length; j++)
        {
            char left = (j > 0) ? curr[j - 1] : '.';
            char right = (j < length - 1) ? curr[j + 1] : '.';

            other[j] = (left == right) ? '.' : '^';
            if (other[j] == '.')
                nextLineCount++;
        }

        std::swap(curr, other);
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
