#include <iostream>
#include <cassert>
#include "Utils.h"

std::pair<int, int> pos, dirn = std::make_pair(1, 0);

// Absolute UDLR or Relative FB using dir (optional number of times)
void move_pos(char c, int n)
{
    switch (c)
    {
    case 'U': pos.second -= n; break;
    case 'D': pos.second += n; break;
    case 'L': pos.first -= n; break;
    case 'R': pos.first += n; break;
    case 'F': pos.first += dirn.first * n; pos.second += dirn.second * n; break;
    case 'B': pos.first -= dirn.first * n; pos.second -= dirn.second * n; break;
    default: assert(false);
    }
}

// Change direction facing: turn LR, or B (invert backwards 180 degrees)
void turn_dirn(char c)
{
    switch (c)
    {
    case 'L': dirn = std::make_pair(dirn.second, -dirn.first); break; // Assumes dirn is (x,y)
    case 'R': dirn = std::make_pair(-dirn.second, dirn.first); break; // Assumes dirn is (x,y)
    case 'B': dirn = std::make_pair(-dirn.first, -dirn.second); break;
    default: assert(false);
    }

    // If using row,col instead of x,y, use this instead:
    // switch (c)
    // {
    // case 'L': dirn = std::make_pair(-dirn.second, dirn.first); break; // Assumes dirn is (row,col)
    // case 'R': dirn = std::make_pair(dirn.second, -dirn.first); break; // Assumes dirn is (row,col)
    // case 'B': dirn = std::make_pair(-dirn.first, -dirn.second); break;
    // default: assert(false);
    // }
}

void test_udlr()
{
    cout << "Move commands: MU, MD, ML, MR, MF, MB [dist]; Turn commands: TL, TR, TB; Exit: X" << endl << endl;

    string line;
    char c = ' ';
    int n = 0;
    for (;;)
    {
        cout << "Current pos is (" << pos.first << "," << pos.second << ") and dir is (" << dirn.first << "," << dirn.second << ")." << endl;
        while (!std::getline(cin, line));
        n = 1;
        std::istringstream ss(line);
        ss >> c;

        c = toupper(c);
        switch (toupper(c))
        {
        case 'X':
            return;
        case 'M':
            n = 1;
            ss >> c >> n;
            move_pos(toupper(c), n);
            break;
        case 'T':
            ss >> c;
            turn_dirn(toupper(c));
            break;
        default:
            return;
        }
    }
}
