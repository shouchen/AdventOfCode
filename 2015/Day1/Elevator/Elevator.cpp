// Elevator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cassert>

void _tmain(int argc, _TCHAR* argv[])
{
    std::ifstream f;
    f.open("Input.txt");

    auto floor = 0, firstBasementStep = 0, currentStep = 0;
    char button;

    while (f >> button)
    {
        currentStep++;

        switch (button)
        {
        case '(': floor++; break;
        case ')': --floor; break;
        default: assert(false);
        }

        if (!firstBasementStep && floor < 0)
            firstBasementStep = currentStep;
    }

    std::cout << "part one: " << floor << std::endl;
    assert(floor == 138);

    std::cout << "part two: " << firstBasementStep << std::endl;
    assert(firstBasementStep == 1771);
}
