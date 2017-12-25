#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <set>

enum class State { A, B, C, D, E, F };

std::set<unsigned> ones;

unsigned get_value(unsigned index)
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
    State state = State::A;
    int index = 0;

    for (auto i = 0; i < 12481997; i++)
    {
        switch (state)
        {
        case State::A:
            if (get_value(index) == 0)
            {
                set_value(index++, 1);
                state = State::B;
            }
            else
            {
                set_value(index--, 0);
                state = State::C;
            }
            break;
        case State::B:
            if (get_value(index) == 0)
            {
                set_value(index--, 1);
                state = State::A;
            }
            else
            {
                set_value(index++, 1);
                state = State::D;
            }
            break;
        case State::C:
            if (get_value(index) == 0)
            {
                set_value(index--, 0);
                state = State::B;
            }
            else
            {
                set_value(index--, 0);
                state = State::E;
            }
            break;
        case State::D:
            if (get_value(index) == 0)
            {
                set_value(index++, 1);
                state = State::A;
            }
            else
            {
                set_value(index++, 0);
                state = State::B;
            }
            break;
        case State::E:
            if (get_value(index) == 0)
            {
                set_value(index--, 1);
                state = State::F;
            }
            else
            {
                set_value(index--, 1);
                state = State::C;
            }
            break;
        case State::F:
            if (get_value(index) == 0)
            {
                set_value(index++, 1);
                state = State::D;
            }
            else
            {
                set_value(index++, 1);
                state = State::A;
            }
            break;
        }
    }

    auto part1 = ones.size();
    std::cout << "Part 1: " << part1 << std::endl;

    assert(part1 == 3362);
    return 0;
}
