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
            auto operand = std::stoull(worksheet[row][col]);
            auto operation = worksheet.back()[col];

            if (operation == "+")
                temp += operand;
            else
                temp *= operand;
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
    auto operation = ' ';

    for (auto col = 0; col < worksheet[0].size(); col++)
    {
        auto cell = worksheet.back()[col];
        if (cell != ' ')
        {
            operation = cell;
            temp = cell == '+' ? 0 : 1;
        }

        auto operand_str = std::string();

        for (auto row = 0; row < worksheet.size() - 1; row++)
            if (isdigit(worksheet[row][col]))
                operand_str.push_back(worksheet[row][col]);

        if (operand_str.length())
        {
            auto operand = std::stoull(operand_str);

            if (operation == '+')
                temp += operand;
            else
                temp *= operand;
        }

        if (operand_str.length() == 0 || col == worksheet[0].size() - 1)
            retval += temp;
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
