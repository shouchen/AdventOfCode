// XmasLights.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <memory>

using namespace std;

class Grid1
{
public:
    Grid1()
    {
        for (int row = 0; row < 1000; row++)
            for (int col = 0; col < 1000; col++)
                data[row][col] = 0;
    }

    virtual void TurnOn(int row, int col)  { data[row][col] = 1; }
    virtual void TurnOff(int row, int col) { data[row][col] = 0; }
    virtual void Toggle(int row, int col)  { data[row][col] = 1 - data[row][col]; }

    int GetTotal()
    {
        auto count = 0;

        for (int row = 0; row < 1000; row++)
            for (int col = 0; col < 1000; col++)
                count += data[row][col];

        return count;
    }

protected:
    int data[1000][1000];
};

class Grid2 : public Grid1
{
public:
    void TurnOn(int row, int col) override  { data[row][col]++; }
    void TurnOff(int row, int col) override { if (data[row][col]) --data[row][col]; }
    void Toggle(int row, int col) override  { data[row][col] += 2; }
};

int ProcessFile(Grid1 *grid)
{
    ifstream f("Input.txt");

    while (f.good())
    {
        string word;
        if (f >> word)
        {
            void (Grid1::*action)(int row, int col) = &Grid1::Toggle;

            if (word == "turn")
            {
                f >> word;
                if (word == "on")
                    action = &Grid1::TurnOn;
                else if (word == "off")
                    action = &Grid1::TurnOff;
                else
                    assert(false);
            }
            else
                assert(word == "toggle");

            int row1, col1, row2, col2;
            char comma;
            string through;
            f >> col1 >> comma >> row1 >> through >> col2 >> comma >> row2;

            for (int row = row1; row <= row2; row++)
                for (int col = col1; col <= col2; col++)
                    (grid->*action)(row, col);
        }
    }

    f.close();

    return grid->GetTotal();
}

void _tmain(int argc, _TCHAR *argv[])
{
    auto grid1 = std::make_unique<Grid1>();
    cout << "part 1: " << ProcessFile(grid1.get()) << endl;

    auto grid2 = std::make_unique<Grid2>();
    cout << "part 2: " << ProcessFile(grid2.get()) << endl;
}
