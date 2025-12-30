#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

auto do_part1(const std::string &filename)
{
    std::vector<std::vector<std::string>> worksheet;

    std::ifstream file(filename);
    std::string line, token;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        worksheet.push_back(std::vector<std::string>());

        while (ss >> token)
            worksheet.back().push_back(token);
    }

    auto retval = 0LL;

    for (auto col = 0; col < worksheet[0].size(); col++)
    {
        auto temp = std::stoull(worksheet[0][col]);

        for (auto row = 1; row < worksheet.size() - 1; row++)
        {
            if (worksheet.back()[col] == "+")
                temp += std::stoull(worksheet[row][col]);
            else
                temp *= std::stoull(worksheet[row][col]);
        }

        retval += temp;
    }

    return retval;
}

auto do_part2(const std::string &filename)
{
    std::vector<std::string> worksheet;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        worksheet.push_back(line);

    auto temp = 0ULL, retval = 0ULL;

    for (int col = worksheet[0].length() - 1; col >= 0; --col)
    {
        auto operation = ' ';
        for (auto i = col; ; --i)
        {
            if (worksheet.back()[i] == '+' || worksheet.back()[i] == '*')
            {
                operation = worksheet.back()[i];
                break;
            }
        }

        auto operand = 0ULL;

        for (auto row = 0; row < worksheet.size() - 1; row++)
            if (isdigit(worksheet[row][col]))
                operand = operand * 10 + (worksheet[row][col] - '0');

        if (temp == 0)
            temp = operand;
        else if (operation == '+')
            temp += operand;
        else
            temp *= operand;

        if (worksheet.back()[col] == operation)
        {
            --col;
            retval += temp;
            temp = 0ULL;
        }
    }

    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 6209956042374);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 12608160008022);

    return 0;
}
