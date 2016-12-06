// Day2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#define PAD_SIZE 5

const char pad1[PAD_SIZE][PAD_SIZE] =
{
    { '1', '2', '3', ' ', ' ' },
    { '4', '5', '6', ' ', ' ' },
    { '7', '8', '9', ' ', ' ' },
    { ' ', ' ', ' ', ' ', ' ' },
    { ' ', ' ', ' ', ' ', ' ' },
};

const char pad2[PAD_SIZE][PAD_SIZE] =
{
    { ' ', ' ', '1', ' ', ' ' },
    { ' ', '2', '3', '4', ' ' },
    { '5', '6', '7', '8', '9' },
    { ' ', 'A', 'B', 'C', ' ' },
    { ' ', ' ', 'D', ' ', ' ' },
};

auto x = 1, y = 1;   // y1 is already defined in math.h!
auto x2 = 0, y2 = 2;

std::string answer1, answer2;

void process_line(const std::string &s, const char pad[PAD_SIZE][PAD_SIZE], int &x, int &y, std::string &answer)
{
    for (char c : s)
    {
        switch (c)
        {
        case 'U': if (y > 0 && pad[y - 1][x] != ' ') y -= 1; break;
        case 'D': if (y < PAD_SIZE - 1 && pad[y + 1][x] != ' ') y += 1; break;
        case 'L': if (x > 0 && pad[y][x - 1] != ' ') x -= 1; break;
        case 'R': if (x < PAD_SIZE - 1 && pad[y][x + 1] != ' ') x += 1; break;
        }
    }

    answer += pad[y][x];
}

int main()
{
    std::ifstream f;
    f.open("input.txt");

    std::string line;

    while (f >> line)
    {
        process_line(line, pad1, x, y, answer1);
        process_line(line, pad2, x2, y2, answer2);
    }

    std::cout << "Part One: " << answer1 << std::endl;
    std::cout << "Part Two: " << answer2 << std::endl;

    assert(answer1 == "84452");
    assert(answer2 == "D65C3");
    return 0;
}
    