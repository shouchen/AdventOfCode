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
    void MoveUp() { if (y > 0 && pad_config[y - 1][x] != ' ') y -= 1; }
    void MoveDown() { if (y < PAD_SIZE - 1 && pad_config[y + 1][x] != ' ') y += 1; }
    void MoveLeft() { if (x > 0 && pad_config[y][x - 1] != ' ') x -= 1; }
    void MoveRight() { if (x < PAD_SIZE - 1 && pad_config[y][x + 1] != ' ') x += 1; }

    const char(*pad_config)[PAD_SIZE];
    int x = 1, y = 1;
};

PadTraversal traversal1(pad_config1, 1, 1);
PadTraversal traversal2(pad_config2, 0, 2);

std::string answer1, answer2;

void process_line(const std::string &s, PadTraversal &traversal, std::string &answer)
{
    for (char c : s)
    {
        switch (c)
        {
        case 'U': traversal.MoveUp(); break;
        case 'D': traversal.MoveDown(); break;
        case 'L': traversal.MoveLeft(); break;
        case 'R': traversal.MoveRight(); break;
        }
    }

    answer += traversal.pad_config[traversal.y][traversal.x];
}

int main()
{
    std::ifstream f;
    f.open("input.txt");

    std::string line;

    while (f >> line)
    {
        process_line(line, traversal1, answer1);
        process_line(line, traversal2, answer2);
    }

    std::cout << "Part One: " << answer1 << std::endl;
    std::cout << "Part Two: " << answer2 << std::endl;

    assert(answer1 == "84452");
    assert(answer2 == "D65C3");
    return 0;
}
    