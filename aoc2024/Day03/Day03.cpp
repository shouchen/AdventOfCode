#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <cassert>

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::regex r(R"(do\(\)|don't\(\)|mul\(([1-9]\d{0,2}),([1-9]\d{0,2})\))");
    std::pair<int, int> retval;
    auto allow = true;

    while (std::getline(file, line))
        for (std::sregex_iterator it(line.begin(), line.end(), r), end; it != end; it++)
        {
            auto token = it->str();

            if (token == "do()")
                allow = true;
            else if (token == "don't()")
                allow = false;
            else
            {
                auto comma = token.find(",");
                auto a = std::atoi(token.c_str() + 4);
                auto b = std::atoi(token.c_str() + comma + 1);
                auto product = a * b;

                retval.first += product;
                if (allow)
                    retval.second += product;
            }
        }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 187825547);
    assert(answer.second == 85508223);
    return 0;
}
