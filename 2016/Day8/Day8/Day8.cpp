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
    void Rect(unsigned width, unsigned height);
    void RotateRow(unsigned row, unsigned times);
    void RotateColumn(unsigned column, unsigned times);
    unsigned Count() const;

private:
    void ReverseColumn(unsigned column, unsigned startRow, unsigned endRow);

    static const unsigned NumRows = 6;
    static const unsigned NumColumns = 50;
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

void Board::Rect(unsigned width, unsigned height)
{
    for (auto row = 0U; row < height; row++)
        for (auto column = 0U; column < width; column++)
            data[row][column] = On;
}

void Board::RotateRow(unsigned row, unsigned times)
{
    auto split = &data[row][NumColumns - times];
    std::reverse(&data[row][0], split);
    std::reverse(split, &data[row][NumColumns]);
    std::reverse(&data[row][0], &data[row][NumColumns]);
}

void Board::RotateColumn(unsigned column, unsigned times)
{
    auto split = NumRows - times;
    ReverseColumn(column, 0, split - 1);
    ReverseColumn(column, split, NumRows - 1);
    ReverseColumn(column, 0, NumRows - 1);
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

void Board::ReverseColumn(unsigned column, unsigned startRow, unsigned endRow)
{
    for (auto top = startRow, bottom = endRow; top < bottom; top++, bottom--)
        std::swap(data[top][column], data[bottom][column]);
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
