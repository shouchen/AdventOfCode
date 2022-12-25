#include <iostream>
#include <fstream>
#include <cassert>

const std::string digits = "=-012";

auto snafu_to_decimal(const std::string &snafu)
{
    auto place = 1LL, decimal = 0LL;
    for (auto d = snafu.rbegin(); d != snafu.rend(); d++)
    {
        auto n = digits.find(*d) - 2;
        decimal += n * place;
        place *= 5;
    }

    return decimal;
}

auto decimal_to_snafu(long long decimal)
{
    std::string snafu;

    for (; decimal; decimal /= 5)
    {
        auto digit_index = (decimal + 2) % 5;
        snafu.push_back(digits[digit_index]);
        decimal += 2 - digit_index;
    }

    std::reverse(snafu.begin(), snafu.end());
    return snafu;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto sum = 0LL;

    while (file >> line)
        sum += snafu_to_decimal(line);

    return decimal_to_snafu(sum);
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    assert(part1 == "2-0-020-1==1021=--01");
    return 0;
}
