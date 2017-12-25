#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <set>

enum State { StateA, StateB, StateC, StateD, StateE, StateF };

std::set<unsigned> ones;

unsigned value_at(unsigned index)
{
    return ones.find(index) == ones.end() ? 0 : 1;
}

void set_value(unsigned index, unsigned value)
{
    if (value)
        ones.insert(index);
    else
        ones.erase(index);
}

int main()
{
    State state = StateA;
    int index = 0;

    for (auto i = 0; i < 12481997; i++)
    {
        switch (state)
        {
        case StateA:
            if (value_at(index) == 0)
            {
                set_value(index, 1);
                index++;
                state = StateB;
            }
            else
            {
                set_value(index, 0);
                index--;
                state = StateC;
            }
            break;
        case StateB:
            if (value_at(index) == 0)
            {
                set_value(index, 1);
                index--;
                state = StateA;
            }
            else
            {
                set_value(index, 1);
                index++;
                state = StateD;
            }
            break;
        case StateC:
            if (value_at(index) == 0)
            {
                set_value(index, 0);
                index--;
                state = StateB;
            }
            else
            {
                set_value(index, 0);
                index--;
                state = StateE;
            }
            break;
        case StateD:
            if (value_at(index) == 0)
            {
                set_value(index, 1);
                index++;
                state = StateA;
            }
            else
            {
                set_value(index, 0);
                index++;
                state = StateB;
            }
            break;
        case StateE:
            if (value_at(index) == 0)
            {
                set_value(index, 1);
                index--;
                state = StateF;
            }
            else
            {
                set_value(index, 1);
                index--;
                state = StateC;
            }
            break;
        case StateF:
            if (value_at(index) == 0)
            {
                set_value(index, 1);
                index++;
                state = StateD;
            }
            else
            {
                set_value(index, 1);
                index++;
                state = StateA;
            }
            break;
        }
    }

    auto part1 = ones.size();

    std::cout << part1 << std::endl;

    assert(part1 == 3362);
    return 0;
}
