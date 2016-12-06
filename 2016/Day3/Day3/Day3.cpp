#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cassert>

int answer1 = 0, answer2 = 0;

bool is_triangle(unsigned a, unsigned b, unsigned c)
{
    return (a + b > c) && (a + c > b) && (b + c > a);
}

void process_file(const char *filename)
{
    std::ifstream file(filename);
    unsigned a, b, c, d, e, f, g, h, i;

    while (file >> a >> b >> c >> d >> e >> f >> g >> h >> i)
    {
        if (is_triangle(a, b, c)) answer1++;
        if (is_triangle(d, e, f)) answer1++;
        if (is_triangle(g, h, i)) answer1++;

        if (is_triangle(a, d, g)) answer2++;
        if (is_triangle(b, e, h)) answer2++;
        if (is_triangle(c, f, i)) answer2++;
    }
}

int main()
{
    process_file("input.txt");

    std::cout << "Part One: " << answer1 << std::endl;
    std::cout << "Part Two: " << answer2 << std::endl;

    assert(answer1 == 869);
    assert(answer2 == 1544);
    return 0;
}
