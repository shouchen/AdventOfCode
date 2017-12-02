#include "stdafx.h"
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

typedef int(*Evaluator)(const std::vector<int> &data);

int row_evaluator1(const std::vector<int> &data)
{
    auto minmax = std::minmax_element(data.cbegin(), data.cend());
    return *minmax.second - *minmax.first;
}

int row_evaluator2(const std::vector<int> &data)
{
    for (auto i = 0U; i < data.size(); i++)
    {
        for (auto j = 0U; j < data.size(); j++)
        {
            if (i == j) continue;

            auto quotient = data[i] / data[j];
            if (quotient == (double)data[i] / data[j])
                return quotient;
        }
    }

    return 0;
}

int process_file(const std::string &filename, Evaluator evaluator)
{
    std::ifstream f(filename);
    std::string input;
    int n, sum = 0;

    while (std::getline(f, input))
    {
        std::stringstream ss(input);
        std::vector<int> data;

        while (ss >> n)
            data.push_back(n);

        sum += evaluator(data);
    }

    return sum;
}

int main()
{
    assert(process_file("input-test.txt", row_evaluator1) == 18);
    assert(process_file("input-test2.txt", row_evaluator2) == 9);

    auto part1 = process_file("input.txt", row_evaluator1);
    auto part2 = process_file("input.txt", row_evaluator2);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 44216);
    assert(part2 == 320);
    return 0;
}
