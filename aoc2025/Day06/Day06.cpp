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

        std::cout << temp << std::endl;
        retval += temp;
    }

    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 6209956042374);

    //auto part2 = do_part2();
    //std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
