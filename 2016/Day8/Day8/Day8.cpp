#include "stdafx.h"
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
    static const int NUM_ROWS = 6;
    static const int NUM_COLUMNS = 50;
    static const char on = '#';
    static const char off = ' ';
    char data[NUM_ROWS][NUM_COLUMNS];

    friend std::ostream &operator<<(std::ostream &o, const Board &b);
};

Board::Board()
{
    for (auto row = 0U; row < NUM_ROWS; row++)
        for (auto column = 0U; column < NUM_COLUMNS; column++)
            data[row][column] = off;
}

void Board::Rect(unsigned a, unsigned b)
{
    for (auto row = 0U; row < b; row++)
        for (auto column = 0U; column < a; column++)
            data[row][column] = on;
}

void Board::RotateRow(unsigned a, unsigned b)
{
    while (b--)
    {
        auto temp = data[a][NUM_COLUMNS - 1];
        for (auto j = NUM_COLUMNS - 1; j > 0; j--)
            data[a][j] = data[a][j - 1];
        data[a][0] = temp;
    }
}

void Board::RotateColumn(unsigned a, unsigned b)
{
    while (b--)
    {
        auto temp = data[NUM_ROWS - 1][a];
        for (auto j = NUM_ROWS - 1; j > 0; j--)
            data[j][a] = data[j - 1][a];
        data[0][a] = temp;
    }
}

unsigned Board::Count() const
{
    auto count = 0U;
    for (auto row = 0U; row < NUM_ROWS; row++)
        for (auto column = 0U; column < NUM_COLUMNS; column++)
            if (data[row][column] == on)
                count++;

    return count;
}

std::ostream &operator<<(std::ostream &o, const Board &bb)
{
    for (auto i = 0U; i < bb.NUM_ROWS; i++)
    {
        for (auto j = 0U; j < bb.NUM_COLUMNS; j++)
            o << bb.data[i][j];
        o << std::endl;
    }

    return o;
}

void process_file(const std::string &filename, Board &board)
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
    process_file("input.txt", board);

    auto count = board.Count();

    std::cout << "Part One: " << count << std::endl;
    std::cout << "Part Two: " << std::endl << std::endl << board;

    assert(count == 106);
    // Part 2: "CFLELOYFCS" (visually inspect the program output)
    return 0;
}
