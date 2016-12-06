// Day3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cassert>

bool is_triangle(unsigned a, unsigned b, unsigned c)
{
    return (a + b > c) && (a + c > b) && (b + c > a);
}

unsigned do_part1()
{
    std::ifstream file;
    file.open("input.txt");

    unsigned a, b, c;
    unsigned num_valid = 0;

    while (file >> a >> b >> c)
    {
        if (is_triangle(a, b, c)) num_valid++;
    }

    return num_valid;
}

unsigned do_part2()
{
    std::ifstream file;
    file.open("input.txt");

    unsigned a, b, c, d, e, f, g, h, i;
    unsigned num_valid = 0;

    while (file >> a >> b >> c >> d >> e >> f >> g >> h >> i)
    {
        if (is_triangle(a, d, g)) num_valid++;
        if (is_triangle(b, e, h)) num_valid++;
        if (is_triangle(c, f, i)) num_valid++;
    }

    return num_valid;
}

int main()
{
    auto num_valid = do_part1();
    std::cout << "Part One: " << num_valid << std::endl;
    assert(num_valid == 869);

    num_valid = do_part2();
    std::cout << "Part Two: " << num_valid << std::endl;
    assert(num_valid == 1544);

    return 0;
}
