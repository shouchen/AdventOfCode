#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

std::string apply_once(std::string data, const std::string &input)
{
    std::istringstream f(input);
    char cmd, a, b, slash, comma;
    unsigned m, n;

    while (f >> cmd)
    {
        switch (cmd)
        {
        case 's':
            f >> n;
            std::rotate(data.begin(), data.end() - n, data.end());
            break;
        case 'x':
            f >> m >> slash >> n;
            std::swap(data[m], data[n]);
            break;
        case 'p':
            f >> a >> slash >> b;
            std::swap(data[data.find(a)], data[data.find(b)]);
            break;
        default:
            assert(false);
        }

        if (!(f >> comma))
            break;
    }

    return data;
}

std::string apply_multiple(std::string data, const std::string &input, unsigned n)
{
    // Record outputs. Pray for a short cycle. Repeat.
    std::vector<std::string> output;

    for (auto i = 0U; i < n; i++)
    {
        output.push_back(data);

        if (data == output[0] && i)
            return output[n % i];

        data = apply_once(data, input);
    }

    return data;
}

int main()
{
    std::string input;
    std::ifstream f_test("input-test.txt");
    f_test >> input;

    auto start = "abcde";
    assert(apply_once(start, input) == "baedc");
    assert(apply_multiple(start, input, 2) == "ceadb");

    std::ifstream f("input.txt");
    f >> input;

    start = "abcdefghijklmnop";
    auto part1 = apply_once(start, input);
    auto part2 = apply_multiple(start, input, 1000000000);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == "gkmndaholjbfcepi");
    assert(part2 == "abihnfkojcmegldp");
    return 0;
}
