#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <map>

std::map<std::pair<int, int>, unsigned> board;

unsigned sum_neighbors(int x, int y)
{
    unsigned sum = 0;
 
    if (board.find({ x - 1, y - 1}) != board.end())
        sum += board[{x - 1, y - 1}];
    if (board.find({ x, y - 1 }) != board.end())
        sum += board[{x, y - 1}];
    if (board.find({ x + 1, y - 1 }) != board.end())
        sum += board[{x + 1, y - 1}];
    if (board.find({ x - 1, y }) != board.end())
        sum += board[{x - 1, y}];
    if (board.find({ x + 1, y }) != board.end())
        sum += board[{x + 1, y}];
    if (board.find({ x - 1, y + 1}) != board.end())
        sum += board[{x - 1, y + 1}];
    if (board.find({ x, y + 1}) != board.end())
        sum += board[{x, y + 1}];
    if (board.find({ x + 1, y + 1}) != board.end())
        sum += board[{x + 1, y + 1}];

    std::cout << "(" << x << "," << y << ") " << sum << std::endl;

    if (sum > 347991)
        std::cout << std::endl; // 349975

    return sum;
}

void do_part2()
{
    int top = 0, bottom = 0, left = 0, right = 1;

    board[{0, 0}] = 1;
    int x = 1, y = 0;

    for (;;)
    {
        // up
        do {
            board[{x, y}] = sum_neighbors(x, y);
        } while (--y >= top);
        top--;

        // left
        do {
            board[{x, y}] = sum_neighbors(x, y);
        } while (--x >= left);
        left--;

        // down
        do {
            board[{x, y}] = sum_neighbors(x, y);
        } while (++y <= bottom);
        bottom++;

        // right
        do {
            board[{x, y}] = sum_neighbors(x, y);
        } while (++x <= right);
        right++;
    }

    //std::cout << "Part Two: " << part2 << std::endl;
}

int main()
{
    do_part2();
    return 0;


    int top = 0, bottom = 0, left = 0, right = 0;
    std::map<unsigned, int> dist;

    dist[1] = 0;
    int x = 0, y = 0;
    unsigned n = 1;

    for (;;)
    {
        // right
        do {
            dist[n++] = abs(x++) + abs(y);
        } while (x <= right);
        right++;

        if (n > 347991) break;

        // up
        do {
            dist[n++] = abs(x) + abs(y--);
        } while (y >= top);
        top--;

        if (n > 347991) break;

        // left
        do {
            dist[n++] = abs(x--) + abs(y);
        } while (x >= left);
        left--;

        if (n > 347991) break;

        // down
        do {
            dist[n++] = abs(x) + abs(y++);
        } while (y <= bottom);
        bottom++;

        if (n > 347991) break;
    }
    
    assert(dist[1] == 0);
    assert(dist[12] == 3);
    assert(dist[23] == 2);
    assert(dist[1024] == 31);

    auto part1 = dist[347991];
    auto part2 = 347991;

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 480);
    assert(part2 == 347991);
    return 0;
}
