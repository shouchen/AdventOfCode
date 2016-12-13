#include "stdafx.h"
#include <iostream>
#include <climits>
#include <cassert>

template<unsigned X, unsigned Y>
class CubeMaze
{
public:
    CubeMaze(unsigned favorite) : favorite(favorite) {}

    bool Solve(unsigned startX, unsigned startY, unsigned targetX, unsigned targetY, unsigned part2Steps)
    {
        for (auto y = 0; y < Y; y++)
            for (auto x = 0; x < X; x++)
                board[y][x] = IsWall(x, y) ? -1 : INT_MAX;

        this->targetX = targetX;
        this->targetY = targetY;

        board[startY][startX] = 0;
        numReachable = 1;

        part1 = 0, part2 = 0;
        for (auto gen = 1U; !part1 || !part2; gen++)
        {
            auto newReachable = DoGeneration(gen);
            if (!newReachable) return false;

            numReachable += newReachable;

            if (!part1 && board[targetY][targetX] == gen) part1 = gen;
            if (gen == part2Steps) part2 = numReachable;
        }

        return true;
    }

    unsigned GetPart1() { return part1; }
    unsigned GetPart2() { return part2; }

private:
    unsigned DoGeneration(unsigned gen)
    {
        auto newReachable = 0U;

        for (auto y = 0; y < Y; y++)
            for (auto x = 0; x < X; x++)
                if (board[y][x] == gen - 1)
                {
                    if (TryReachNeighbor(x, y - 1, gen)) newReachable++; // UP
                    if (TryReachNeighbor(x, y + 1, gen)) newReachable++; // DOWN
                    if (TryReachNeighbor(x - 1, y, gen)) newReachable++; // LEFT
                    if (TryReachNeighbor(x + 1, y, gen)) newReachable++; // RIGHT
                }

        return newReachable;
    }

    bool TryReachNeighbor(int x, int y, unsigned gen)
    {
        if (x < 0 || x >= X || y < 0 || y >= Y || board[y][x] != INT_MAX)
            return false;

        board[y][x] = gen;
        return true;
    }

    inline bool IsWall(int x, int y)
    {
        return CountSetBits(x * x + 3 * x + 2 * x * y + y + y * y + favorite) & 0x1;
    }

    inline unsigned CountSetBits(unsigned n)
    {
        auto count = 0U;
        while (n)
        {
            count += n & 1;
            n >>= 1;
        }
        return count;
    }

    int board[Y][X];
    unsigned targetX = 0, targetY = 0, favorite, numReachable = 0, part1 = 0, part2 = 0;
};

int main()
{
    CubeMaze<250, 250> cubeMaze(1364);
    auto part1 = 0U, part2 = 0U;

    if (cubeMaze.Solve(1, 1, 31, 39, 50))
    {
        part1 = cubeMaze.GetPart1();
        part2 = cubeMaze.GetPart2();

        std::cout << "Part One: " << cubeMaze.GetPart1() << std::endl;
        std::cout << "Part Two: " << cubeMaze.GetPart2() << std::endl;
    }

    assert(part1 == 86);
    assert(part2 == 127);
    return 0;
}
