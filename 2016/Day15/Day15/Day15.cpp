#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cassert>

int time;

struct Disc
{
    Disc(int numPositions, int currPosition) : numPositions(numPositions), currPosition(currPosition) {}
    int numPositions;
    int currPosition;
};

std::map<int, Disc *> discs;

void DoTick(int num = 1)
{
    time += num;
    for (auto &d : discs)
        d.second->currPosition = (d.second->currPosition + num) % d.second->numPositions;
}

bool PressButton(int time)
{
    DoTick(time);

    DoTick();

    if (discs[1]->currPosition != 0)
        return false;

    DoTick();

    if (discs[2]->currPosition != 0)
        return false;

    return true;
}

void ReadFile(const std::string &filename)
{
    std::ifstream f(filename);
    std::string disc, has, positions, at, time, it, is, position;
    char pound, period, c1;
    int d, p, startPos = 0;

    while (f >> disc >> pound >> d >> has >> p >> positions >> at >> time >> it >> is >> at >> position >> startPos >> period)
    {
        assert(time == "time=0,");
        Disc *tempDisc = new Disc(p, startPos);
        discs[d] = tempDisc;
    }

//    Disc #1 has 5 positions; at time = 0, it is at position 4.


}

int main()
{
    ReadFile("input-test.txt");
    auto result = PressButton(5);

    std::cout << result << std::endl;
    return 0;
}
