// Elevator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

void _tmain(int argc, _TCHAR* argv[])
{
    auto floor = 0;
    auto firstBasementStep = 0;
    auto currentStep = 0;
    char button;

    ifstream f;
    f.open("Input.txt");

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

    f.close();

    cout << floor << " floors" << endl;
    cout << firstBasementStep << " was first basement step" << endl;
}
