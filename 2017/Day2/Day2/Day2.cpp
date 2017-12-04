#include "stdafx.h"
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

class Spreadsheet
{
public:
    Spreadsheet(const std::string &filename);
    unsigned ComputeChecksum1();
    unsigned ComputeChecksum2();

private:
    static unsigned RowEvaluator1(const std::vector<unsigned> row);
    static unsigned RowEvaluator2(const std::vector<unsigned> row);

    std::vector<std::vector<unsigned>> data;
};

Spreadsheet::Spreadsheet(const std::string &filename)
{
    std::ifstream f(filename);
    std::string input;

    while (std::getline(f, input))
    {
        data.push_back(std::vector<unsigned>());
        std::stringstream ss(input);
        unsigned n;

        while (ss >> n)
            data[data.size() - 1].push_back(n);
    }
}

unsigned Spreadsheet::ComputeChecksum1()
{
    auto sum = 0U;
    for (auto row : data)
        sum += RowEvaluator1(row);

    return sum;
}

unsigned Spreadsheet::ComputeChecksum2()
{
    auto sum = 0U;
    for (auto row : data)
        sum += RowEvaluator2(row);

    return sum;
}

unsigned Spreadsheet::RowEvaluator1(const std::vector<unsigned> row)
{
    auto minmax = std::minmax_element(row.cbegin(), row.cend());
    return *minmax.second - *minmax.first;
}

unsigned Spreadsheet::RowEvaluator2(const std::vector<unsigned> row)
{
    for (auto i = 0U; i < row.size(); i++)
    {
        for (auto j = 0U; j < row.size(); j++)
        {
            if (i == j) continue;

            auto quotient = row[i] / row[j];
            if (quotient == (double)row[i] / row[j])
                return quotient;
        }
    }

    return 0;
}

void check_test_cases()
{
    Spreadsheet input_test("input-test.txt");
    assert(input_test.ComputeChecksum1() == 18);

    Spreadsheet input_test2("input-test2.txt");
    assert(input_test2.ComputeChecksum2() == 9);
}

int main()
{
    check_test_cases();

    Spreadsheet input("input.txt");
    auto part1 = input.ComputeChecksum1();
    auto part2 = input.ComputeChecksum2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 44216);
    assert(part2 == 320);
    return 0;
}
