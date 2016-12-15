#include "stdafx.h"
#include <iostream>
#include <set>
#include <queue>
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

    //Elerium_M = 0x0A,
    //Elerium_G = 0x0B,
    //Dilithium_M = 0x0C,
    //Dilithium_G = 0x0D,

    //Hydrogen_M = 0x00,
    //Hydrogen_G = 0x01,
    //Lithium_M = 0x02,
    //Lithium_G = 0x03,

    NumNames
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
    State() : value(0) { SetElevatorFloor(1); }

    inline unsigned GetHash() const { return value; }
    inline unsigned GetElevatorFloor() const { return ((value >> 28) & 0x03) + 1; }
    inline void SetElevatorFloor(unsigned floor) { value = (value & 0xcfffffff) | ((floor - 1) << 28); }
    inline void AdjustElevatorFloor(int direction) { SetElevatorFloor(GetElevatorFloor() + direction); }
    inline unsigned GetFloor(Name name) const { return ((value >> (name * 2)) & 0x03) + 1; }
    inline void SetFloor(Name name, unsigned floor) { value = (value & ~(0x03U << (name * 2))) | ((floor - 1U) << (name * 2)); }
    inline void AdjustFloor(Name name, int direction) { SetFloor(name, GetFloor(name) + direction); }

    bool IsValid()
    {
        // See if any chips will "fry"; loop through all chips.
        for (auto i = 0; i < NumNames; i++)
        {
            Name name1(static_cast<Name>(i));
            if ((name1 & 0x01) == 0) // a chip
            {
                bool withCorrespondingGenerator = false, withDifferentGenerator = false;

                // Look through all generators on the same floor
                for (auto j = 0; j < NumNames; j++)
                {
                    Name name2(static_cast<Name>(j));
                    if (GetFloor(name1) == GetFloor(name2))
                    {
                        if ((name1 ^ name2) == 1)
                            withCorrespondingGenerator = true;
                        else if (name2 & 0x01)
                            withDifferentGenerator = true;
                    }
                }

                if (withDifferentGenerator && !withCorrespondingGenerator)
                    return false;
            }
        }

        return true;
    }

    bool IsSolved()
    {
        for (auto i = 0; i < NumNames; i++)
            if (GetFloor(static_cast<Name>(i)) != 4)
                return false;

        return true;
    }

private:
    unsigned value;
};

struct QueueNode
{
    State state;
    unsigned numMoves;
};

std::set<unsigned> seen;
std::queue<QueueNode> queue;

unsigned best_solution = INT_MAX;

void ScheduleSecond(QueueNode node, Name name, int direction)
{
    node.state.AdjustFloor(name, direction);

    if (node.numMoves < best_solution && node.state.IsValid())
    {
        queue.push(node);
    }
}

void ScheduleFirst(QueueNode node, Name name1, int direction)
{
    int elevator = node.state.GetElevatorFloor();

    node.state.AdjustFloor(name1, direction);
    node.state.AdjustElevatorFloor(direction);
    node.numMoves++;

    if (node.numMoves >= best_solution) return;

    if (node.state.IsValid())
        queue.push(node);

    for (auto i = name1 + 1; i < NumNames; i++)
    {
        Name name2(static_cast<Name>(i));
        if (node.state.GetFloor(name2) == elevator)
            ScheduleSecond(node, name2, direction);
    }
}

void ScheduleUpOrDown(QueueNode &node, int direction)
{
    for (auto i = 0; i < NumNames; i++)
    {
        Name name(static_cast<Name>(i));
        if (node.state.GetFloor(name) == node.state.GetElevatorFloor())
            ScheduleFirst(node, name, direction);
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
        if (node.numMoves < best_solution)
            best_solution = node.numMoves;

        return false;
    }

    if (node.state.GetElevatorFloor() != 4) ScheduleUpOrDown(node, 1);
    if (node.state.GetElevatorFloor() != 1) ScheduleUpOrDown(node, -1);

    return false;
}

int main()
{
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

    std::cout << "Solution: " << best_solution << std::endl;

    assert(best_solution == 47);   // Part 1
    //assert(best_solution == 71); // Part 2
    //assert(best_solution == 11); // Example
    return 0;
}
