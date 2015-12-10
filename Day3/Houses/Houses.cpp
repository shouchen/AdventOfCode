// Houses.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <set>

using namespace std;

struct Address
{
    int x = 0, y = 0;
};

struct LessAddress
{
    bool operator()(const Address &lhs, const Address &rhs) const
    {
        if (lhs.x < rhs.x) return true;
        if (lhs.x > rhs.x) return false;
        return lhs.y < rhs.y;
    }
};

void ProcessMove(char move, Address &santa)
{
    switch (move)
    {
        case '^': --santa.y; break;
        case 'v': santa.y++; break;
        case '<': --santa.x; break;
        case '>': santa.x++; break;
    }
}

unsigned ProcessFile(const string &filename, bool doubleSanta)
{
    set<const Address, LessAddress> visited;

    Address santa1, santa2;
    visited.insert(santa1);

    ifstream f;
    f.open("Input.txt");

    while (!f.eof())
    {
        char move;
        if (f >> move)
        {
            ProcessMove(move, santa1);
            visited.insert(santa1);

            if (doubleSanta && f >> move)
            {
                ProcessMove(move, santa2);
                visited.insert(santa2);
            }
        }
    }

    f.close();

    return visited.size();
}

void _tmain(int argc, _TCHAR* argv[])
{
    cout << "one santa = " << ProcessFile("Input.txt", false) << endl;
    cout << "two santas = " << ProcessFile("Input.txt", true) << endl;
}
