#include "stdafx.h"
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

typedef int(*Evaluator)(const std::vector<int> &data);

int eval_row1(const std::vector<int> &data)
{
    auto minmax = std::minmax_element(data.cbegin(), data.cend());
    return *minmax.second - *minmax.first;
}

int eval_row2(const std::vector<int> &data)
{
    for (auto i = 0U; i < data.size(); i++)
    {
        for (auto j = 0U; j < data.size(); j++)
        {
            if (i == j) continue;

            auto div = data[i] / data[j];
            if (div == (double)data[i] / data[j])
                return div;
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
    assert(process_file("input-test.txt", eval_row1) == 18);
    assert(process_file("input-test2.txt", eval_row2) == 9);

    auto part1 = process_file("input.txt", eval_row1);
    auto part2 = process_file("input.txt", eval_row2);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 44216);
    assert(part2 == 320);
    return 0;
}
