#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

class Board
{
public:
    Board();
    void Rect(unsigned a, unsigned b);
    void RotateRow(unsigned a, unsigned b);
    void RotateColumn(unsigned a, unsigned b);
    unsigned Count() const;

private:
    void ReverseColumn(unsigned col, unsigned startRow, unsigned endRow);

    static const int NumRows = 6;
    static const int NumColumns = 50;
    static const char On = '#';
    static const char Off = ' ';
    char data[NumRows][NumColumns];

    friend std::ostream &operator<<(std::ostream &o, const Board &b);
};

Board::Board()
{
    for (auto row = 0U; row < NumRows; row++)
        for (auto &cell : data[row])
            cell = Off;
}

void Board::Rect(unsigned a, unsigned b)
{
    for (auto row = 0U; row < b; row++)
        for (auto column = 0U; column < a; column++)
            data[row][column] = On;
}

void Board::RotateRow(unsigned a, unsigned b)
{
    auto split = &data[a][NumColumns - b];
    std::reverse(&data[a][0], split);
    std::reverse(split, &data[a][NumColumns]);
    std::reverse(&data[a][0], &data[a][NumColumns]);
}

void Board::RotateColumn(unsigned a, unsigned b)
{
    auto split = NumRows - b;
    ReverseColumn(a, 0, split - 1);
    ReverseColumn(a, split, NumRows - 1);
    ReverseColumn(a, 0, NumRows - 1);
}

unsigned Board::Count() const
{
    auto count = 0U;
    for (auto row = 0U; row < NumRows; row++)
        for (auto cell : data[row])
            if (cell == On)
                count++;

    return count;
}

void Board::ReverseColumn(unsigned col, unsigned startRow, unsigned endRow)
{
    for (auto top = startRow, bottom = endRow; top < bottom; top++, bottom--)
        std::swap(data[top][col], data[bottom][col]);
}

std::ostream &operator<<(std::ostream &o, const Board &bb)
{
    for (auto i = 0U; i < bb.NumRows; i++)
    {
        for (auto j = 0U; j < bb.NumColumns; j++)
            o << bb.data[i][j];
        o << std::endl;
    }

    return o;
}

void ProcessFile(const std::string &filename, Board &board)
{
    std::ifstream f(filename);
    std::string word, by;
    char x, y, equals;
    unsigned a, b;

    while (f >> word)
    {
        if (word == "rect")
        {
            f >> a >> x >> b;
            board.Rect(a, b);
        }
        else if (word == "rotate")
        {
            f >> word;

            if (word == "row")
            {
                f >> y >> equals >> a >> by >> b;
                board.RotateRow(a, b);
            }
            else if (word == "column")
            {
                f >> x >> equals >> a >> by >> b;
                board.RotateColumn(a, b);
            }
        }
    }
}

int main()
{
    Board board;
    ProcessFile("input.txt", board);

    auto count = board.Count();

    std::cout << "Part One: " << count << std::endl;
    std::cout << "Part Two: " << std::endl << std::endl << board;

    assert(count == 106);
    // Part 2: "CFLELOYFCS" (visually inspect the program output)
    return 0;
}
