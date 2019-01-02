#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std::string_literals;

enum Direction
{
    Down, Right, Up, Left
};

enum Turn
{
    TurnLeft, GoStraight, TurnRight
};

struct Cart
{
    int x, y;
    Direction dir;
    Turn next_turn;
    bool deleted;

    auto move(std::vector<std::string> &grid, std::vector<Cart> &carts)
    {
        if (dir == Down) y++;
        else if (dir == Right) x++;
        else if (dir == Up) --y;
        else if (dir == Left) --x;

        switch (grid[y][x])
        {
        case '\\':
            if (dir == Down) dir = Right;
            else if (dir == Right) dir = Down;
            else if (dir == Up) dir = Left;
            else if (dir == Left) dir = Up;
            break;
        case '/':
            if (dir == Down) dir = Left;
            else if (dir == Right) dir = Up;
            else if (dir == Up) dir = Right;
            else if (dir == Left) dir = Down;
            break;
        case '+':
            switch (next_turn)
            {
            case TurnLeft:
                if (dir == Down) dir = Right;
                else if (dir == Right) dir = Up;
                else if (dir == Up) dir = Left;
                else if (dir == Left) dir = Down;
                break;
            case TurnRight:
                if (dir == Down) dir = Left;
                else if (dir == Right) dir = Down;
                else if (dir == Up) dir = Right;
                else if (dir == Left) dir = Up;
                break;
            }

            if (next_turn == TurnLeft) next_turn = GoStraight;
            else if (next_turn == GoStraight) next_turn = TurnRight;
            else if (next_turn == TurnRight) next_turn = TurnLeft;
            break;
        }

        for (Cart &c : carts)
            if (c.x == x && c.y == y && (c.dir != dir || c.next_turn != next_turn))
            {
                deleted = c.deleted = true;
                return false;
            }

        return true;
    }
};

void read_input(const std::string &filename, std::vector<std::string> &grid, std::vector<Cart> &carts)
{
    std::ifstream file(filename);
    auto line = ""s;

    while (getline(file, line))
        grid.push_back(line);

    for (auto i = 0; i < grid.size(); i++)
    {
        for (auto j = 0; j < grid[0].size(); j++)
        {
            switch (grid[i][j])
            {
            case 'v':
                carts.push_back(Cart{ j, i, Down, TurnLeft, false });
                grid[i][j] = '|';
                break;
            case '>':
                carts.push_back(Cart{ j, i, Right, TurnLeft, false });
                grid[i][j] = '-';
                break;
            case '^':
                carts.push_back(Cart{ j, i, Up, TurnLeft, false });
                grid[i][j] = '|';
                break;
            case '<':
                carts.push_back(Cart{ j, i, Left, TurnLeft, false });
                grid[i][j] = '-';
                break;
            }
        }
    }
}

auto do_part(const std::string &filename, bool for_part2)
{
    std::vector<std::string> grid;
    std::vector<Cart> carts;

    read_input(filename, grid, carts);

    while (carts.size() > 1)
    {
        for (auto &cart : carts)
            if (!cart.move(grid, carts) && !for_part2)
                return std::make_pair(cart.x, cart.y);

        carts.erase(
            std::remove_if(carts.begin(), carts.end(), [&](const Cart &item) { return item.deleted; }),
            carts.end());

        std::sort(carts.begin(), carts.end(), [&](const Cart &lhs, const Cart& rhs) -> bool {
            return std::make_pair(lhs.y, lhs.x) < std::make_pair(rhs.y, rhs.x);
        });
    }

    return std::make_pair(carts[0].x, carts[0].y);
}

int main()
{
    auto part1 = do_part("input.txt", false);
    auto part2 = do_part("input.txt", true);

    std::cout << "Part 1: " << part1.first << "," << part1.second << std::endl;
    std::cout << "Part 2: " << part2.first << "," << part2.second << std::endl;

    assert(part1 == std::make_pair(76, 108));
    assert(part2 == std::make_pair(2, 84));

    return 0;
}
