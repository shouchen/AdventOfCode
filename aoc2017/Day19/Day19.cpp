#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void process_input(const std::string &filename, std::string &path, unsigned &step_count)
{
    path.clear();
    step_count = 0;
    std::vector<std::string> board;

    std::ifstream f(filename);
    std::string line;
    while (getline(f, line))
        board.push_back(line);

    // get start
    int row = 0, col = 0;
    for (col = 0; ; col++)
        if (board[row][col] == '|')
            break;

    int xdir = 0, ydir = 1;

    for (;;)
    {
        // go vertical
        while (board[row][col] == '|' || board[row][col] == '-' || isalpha(board[row][col]))
        {
            step_count++;
            if (isalpha(board[row][col]))
                path.push_back(board[row][col]);
            row += ydir, col += xdir;
        }

        if (board[row][col] != '+')
            break;

        // turn
        if (col > 0 && board[row][col - 1] != ' ') xdir = -1, ydir = 0; // left
        else if (col < board[row].size() - 1 && board[row][col + 1] != ' ') xdir = 1, ydir = 0; //right
        else assert(false);

        row += ydir, col += xdir;
        step_count++;

        // go horizontal
        while (board[row][col] == '|' || board[row][col] == '-' || isalpha(board[row][col]))
        {
            step_count++;
            if (isalpha(board[row][col]))
                path.push_back(board[row][col]);
            row += ydir, col += xdir;
        }

        if (board[row][col] != '+')
            break;

        // turn
        if (row > 0 && board[row - 1][col] != ' ') xdir = 0, ydir = -1; // up
        else if (row < board.size() - 1 && board[row + 1][col] != ' ') xdir = 0, ydir = 1; //right
        else assert(false);

        row += ydir, col += xdir;
        step_count++;
    }
}

int main()
{
    std::string part1;
    unsigned part2;
    process_input("input-test.txt", part1, part2);
    assert(part1 == "ABCDEF");
    assert(part2 == 38);

    process_input("input.txt", part1, part2);
    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == "EPYDUXANIT");
    assert(part2 == 17544);
    return 0;
}
