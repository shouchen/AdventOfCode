#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#define PAD_SIZE 5

const char pad_config1[PAD_SIZE][PAD_SIZE] =
{
    { '1', '2', '3', ' ', ' ' },
    { '4', '5', '6', ' ', ' ' },
    { '7', '8', '9', ' ', ' ' },
    { ' ', ' ', ' ', ' ', ' ' },
    { ' ', ' ', ' ', ' ', ' ' },
};

const char pad_config2[PAD_SIZE][PAD_SIZE] =
{
    { ' ', ' ', '1', ' ', ' ' },
    { ' ', '2', '3', '4', ' ' },
    { '5', '6', '7', '8', '9' },
    { ' ', 'A', 'B', 'C', ' ' },
    { ' ', ' ', 'D', ' ', ' ' },
};

class PadTraversal
{
public:
    PadTraversal(const char pad_config[PAD_SIZE][PAD_SIZE], int x, int y)
        : pad_config(pad_config), x(x), y(y) 
    {
    }
    char process_moves(const std::string &moves);

private:
    const char(*pad_config)[PAD_SIZE];
    int x, y;
};

char PadTraversal::process_moves(const std::string &moves)
{
    for (char c : moves)
    {
        switch (c)
        {
        case 'U': if (y > 0 && pad_config[y - 1][x] != ' ') y -= 1;            break;
        case 'D': if (y < PAD_SIZE - 1 && pad_config[y + 1][x] != ' ') y += 1; break;
        case 'L': if (x > 0 && pad_config[y][x - 1] != ' ') x -= 1;            break;
        case 'R': if (x < PAD_SIZE - 1 && pad_config[y][x + 1] != ' ') x += 1; break;
        }
    }

    return pad_config[y][x];
}

int main()
{
    PadTraversal traversal1(pad_config1, 1, 1);
    PadTraversal traversal2(pad_config2, 0, 2);

    std::ifstream f("input.txt");
    std::string line, answer1, answer2;

    while (f >> line)
    {
        answer1.push_back(traversal1.process_moves(line));
        answer2.push_back(traversal2.process_moves(line));
    }

    std::cout << "Part One: " << answer1 << std::endl;
    std::cout << "Part Two: " << answer2 << std::endl;

    assert(answer1 == "84452");
    assert(answer2 == "D65C3");
    return 0;
}
