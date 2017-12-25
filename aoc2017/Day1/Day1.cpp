#include "stdafx.h"
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>

typedef unsigned(*OtherIndexComputer)(int curr, unsigned length);

auto get_other_index1(int curr, unsigned length)
{
    return (curr + 1) % length;
}

auto get_other_index2(int curr, unsigned length)
{
    return (curr + length / 2) % length;
}

auto compute_captcha(const std::string &input, OtherIndexComputer computer)
{
    auto sum = 0U;

    for (auto i = 0U; i < input.length(); i++)
    {
        auto other_index = computer(i, unsigned(input.length()));
        if (input[i] == input[other_index])
        {
            sum += input[i] - '0';
        }
    }

    return sum;
}

int main()
{
    assert(compute_captcha("1122", get_other_index1) == 3);
    assert(compute_captcha("1111", get_other_index1) == 4);
    assert(compute_captcha("1234", get_other_index1) == 0);
    assert(compute_captcha("91212129", get_other_index1) == 9);

    assert(compute_captcha("1212", get_other_index2) == 6);
    assert(compute_captcha("1221", get_other_index2) == 0);
    assert(compute_captcha("123425", get_other_index2) == 4);
    assert(compute_captcha("123123", get_other_index2) == 12);
    assert(compute_captcha("12131415", get_other_index2) == 4);

    std::ifstream f("input.txt");
    std::string input;
    f >> input;

    auto part_one = compute_captcha(input, get_other_index1);
    auto part_two = compute_captcha(input, get_other_index2);

    std::cout << "Part One: " << part_one << std::endl;
    std::cout << "Part Two: " << part_two << std::endl;

    assert(part_one == 1141);
    assert(part_two == 950);

    return 0;
}
