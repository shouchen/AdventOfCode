#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <set>
#include <queue>
#include <ctime>
#include <cassert>

enum Name : char
{
    Polonium_M = 0x00,
    Polonium_G = 0x01,
    Thulium_M = 0x02,
    Thulium_G = 0x03,
    Promethium_M = 0x04,
    Promethium_G = 0x05,
    Ruthenium_M = 0x06,
    Ruthenium_G = 0x07,
    Cobalt_M = 0x08,
    Cobalt_G = 0x09,

    Elerium_M = 0x0A,
    Elerium_G = 0x0B,
    Dilithium_M = 0x0C,
    Dilithium_G = 0x0D,
};

// To minimize the memory footprint, store the state of the world in a single unsigned int like this:
//
//  3         2         1
// 10987654321098765432109876543210
//     xdxcxbxax9x8x7x6x5x4x3x2x1x0 <= for each object, store (floor - 1) in the two bits
//   ee                             <= for the elevator, store (floor - 1) in these two bits
// --                               <= unused
struct State
{
    State() { SetElevatorFloor(1); }

    inline unsigned long long GetHash() const { return ((unsigned long long)floor[0]) | ((unsigned long long)floor[1] << 16) | ((unsigned long long)floor[2] << 32) | ((unsigned long long)floor[3] << 48); }
    inline unsigned GetElevatorFloor() const 
    {
        for (int i = 0; i <= 3; i++)
            if (floor[i] & 0x8000) return i + 1;
        return 0;
    }
    inline void SetElevatorFloor(unsigned f)
    {
        floor[0] &= ~0x8000; floor[1] &= ~0x8000; floor[2] &= ~0x8000; floor[3] &= ~0x8000;
        floor[f - 1] |= 0x8000;
    }
    inline void AdjustElevatorFloor(int direction)
    {
        SetElevatorFloor(GetElevatorFloor() + direction);
    }
    inline unsigned GetFloor(Name name) const
    {
        unsigned short bit = 1 << name;
        for (int i = 0; i <= 3; i++)
            if (floor[i] & bit) return i + 1;
        return 0;
    }
    inline void SetFloor(Name name, unsigned f)
    {
        unsigned short bit = 1 << name;
        floor[0] &= ~bit; floor[1] &= ~bit; floor[2] &= ~bit; floor[3] &= ~bit;
        floor[f - 1] |= bit;
    }
    inline void AdjustFloor(Name name, int direction)
    {
        SetFloor(name, GetFloor(name) + direction);
    }

    bool IsValid()
    {
        // Make sure no chips will fry
        for (int i = 0; i < 10; i += 2)
        {
            auto chipFloor = GetFloor(static_cast<Name>(i));
            auto genFloor = GetFloor(static_cast<Name>(i + 1));

            if (genFloor == chipFloor) continue; // safe

            // See if other gens are on the same floor as this chip
            if (floor[chipFloor - 1] & 0x2aaaaaaa) return false;
        }

        return true;
    }

    bool IsSolved()
    {
        for (auto i = 0; i < 3; i++)
            if (floor[i] & 0xfe)
                return false;

        return true;
    }

private:
    unsigned short floor[4] = { 0, 0, 0, 0 };
};

void Dump(State state)
{
    for (int i = 4; i > 0; i--)
    {
        std::cout << ((state.GetElevatorFloor() == i) ? 'e' : ' ') << i << ": ";
        for (int j = 0; j < 15; j++)
        {
            if (state.GetFloor(static_cast<Name>(j)) == i)
                if (j & 1)
                    std::cout << char('A' + ((j - 1) / 2));
                else
                    std::cout << char('a' + (j / 2));
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

struct QueueNode
{
    State state;
    unsigned numMoves;
};

std::set<unsigned long long> seen;
std::queue<QueueNode> queue;

unsigned bestSolution = INT_MAX;

void ScheduleSecondObject(QueueNode node, Name name, int direction)
{
    node.state.AdjustFloor(name, direction);

    if (node.numMoves < bestSolution && node.state.IsValid())
        queue.push(node);
}

void ScheduleFirstObject(QueueNode node, Name name1, int direction)
{
    int elevator = node.state.GetElevatorFloor();

    node.state.AdjustFloor(name1, direction);
    node.state.AdjustElevatorFloor(direction);
    node.numMoves++;

    if (node.numMoves >= bestSolution) return;

    if (node.state.IsValid())
        queue.push(node);

    for (auto i = name1 + 1; i < 10; i++)
    {
        Name name2(static_cast<Name>(i));
        if (node.state.GetFloor(name2) == elevator)
            ScheduleSecondObject(node, name2, direction);
    }
}

void ScheduleUpOrDown(QueueNode &node, int direction)
{
    for (auto i = 0; i < 10; i++)
    {
        Name name(static_cast<Name>(i));
        if (node.state.GetFloor(name) == node.state.GetElevatorFloor())
            ScheduleFirstObject(node, name, direction);
    }
}

bool Solve()
{
    if (queue.empty())
        return true;

    QueueNode node = queue.front();
    queue.pop();

    auto hash = node.state.GetHash();
    if (seen.find(hash) != seen.end())
        return false;

    seen.insert(hash);

    if (node.state.IsSolved())
    {
        if (node.numMoves < bestSolution)
            bestSolution = node.numMoves;

        return false;
    }

    if (node.state.GetElevatorFloor() != 4)
        ScheduleUpOrDown(node, 1);

    if (node.state.GetElevatorFloor() != 1)
        ScheduleUpOrDown(node, -1);

    return false;
}

int main()
{
    double startTime = clock();

    State state;

    state.SetFloor(Polonium_M, 2);
    state.SetFloor(Promethium_M, 2);
    state.SetFloor(Polonium_G, 1);
    state.SetFloor(Thulium_G, 1);
    state.SetFloor(Thulium_M, 1);
    state.SetFloor(Promethium_G, 1);
    state.SetFloor(Ruthenium_G, 1);
    state.SetFloor(Ruthenium_M, 1);
    state.SetFloor(Cobalt_G, 1);
    state.SetFloor(Cobalt_M, 1);

    //state.SetFloor(Elerium_G, 1);
    //state.SetFloor(Elerium_M, 1);
    //state.SetFloor(Dilithium_G, 1);
    //state.SetFloor(Dilithium_M, 1);

    //state.SetFloor(Hydrogen_M, 1);
    //state.SetFloor(Hydrogen_G, 2);
    //state.SetFloor(Lithium_M, 1);
    //state.SetFloor(Lithium_G, 3);

    queue.push(QueueNode{ state, 0 });

    while(!Solve());

    std::cout << "Part One: " << bestSolution << std::endl;
    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;

    assert(bestSolution == 47);   // Part 1
    //assert(bestSolution == 71); // Part 2
    return 0;
}
