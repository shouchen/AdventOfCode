#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void solve(const std::string &filename, std::string &path, unsigned &num_steps)
{
    std::vector<std::string> board;
    std::ifstream f(filename);
    std::string line;

    while (getline(f, line))
        board.push_back(line);

    auto x = int(board[0].find('|')), y = 0, x_dir = 0, y_dir = 1;
    path.clear();
    num_steps = 0;

    do
    {
        do
        {
            num_steps++;
            if (isalpha(board[y][x]))
                path.push_back(board[y][x]);
        } while (board[y += y_dir][x += x_dir] != '+' && board[y][x] != ' ');

        if (x_dir)
        {
            x_dir = 0;
            y_dir = (y > 0 && board[y - 1][x] == ' ') ? 1 : -1;
        }
        else
        {
            x_dir = (x > 0 && board[y][x - 1] == ' ') ? 1 : -1;
            y_dir = 0;
        }
    } while (board[y][x] == '+');
}

int main()
{
    std::string part1;
    auto part2 = 0U;

    solve("input-test.txt", part1, part2);
    assert(part1 == "ABCDEF");
    assert(part2 == 38);

    solve("input.txt", part1, part2);
    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == "EPYDUXANIT");
    assert(part2 == 17544);
    return 0;
}
