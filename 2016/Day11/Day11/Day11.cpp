#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <set>
#include <queue>
#include <ctime>
#include <cassert>

// These must be ordered by microchip-generator pairs (microchip followed by
// its corresponding generator).
enum : unsigned short
{
    Polonium_M =   0x0001,
    Polonium_G =   0x0002,
    Thulium_M =    0x0004,
    Thulium_G =    0x0008,
    Promethium_M = 0x0010,
    Promethium_G = 0x0020,
    Ruthenium_M =  0x0040,
    Ruthenium_G =  0x0080,
    Cobalt_M =     0x0100,
    Cobalt_G =     0x0200,
    Elerium_M =    0x0400,
    Elerium_G =    0x0800,
    Dilithium_M =  0x1000,
    Dilithium_G =  0x2000,

    Elevator =     0x8000
};

const unsigned short NotElevator = ~Elevator;
const unsigned short AllMicrochips = 0x1555;
const unsigned short AllGenerators = 0x2aaa;

struct State
{
    inline unsigned GetElevatorIndex() const 
    {
        if (floor[3] & Elevator) return 3;
        if (floor[2] & Elevator) return 2;
        if (floor[1] & Elevator) return 1;
        return 0;
    }
    
    inline void PlaceObjectAtFloor(unsigned short name, unsigned f)
    {
        floor[f - 1] |= name;
    }
    
    inline bool IsObjectAtIndex(unsigned short name, unsigned index) const
    {
        return (floor[index] & name) ? true : false;
    }
    
    inline void MoveObject(unsigned short name, unsigned oldIndex, unsigned newIndex)
    {
        floor[oldIndex] &= ~name;
        floor[newIndex] |= name;
    }

    inline bool IsSolved()
    {
        return !(floor[0] & NotElevator) && !(floor[1] & NotElevator) && !(floor[2] & NotElevator);
    }

    inline bool IsIndexValid(unsigned index)
    {
        unsigned short generators = floor[index] & AllGenerators;
        if (!generators) return true;

        unsigned short microchips = floor[index] & AllMicrochips;
        unsigned short microchipsWithoutTheirGenerator = microchips & ~(generators >> 1);

        return !microchipsWithoutTheirGenerator;
    }

    // Special hash function that hashes to the same value all states that are
    // permutations of each other (but which are otherwise unique ). This
    // dramatically prunes the search tree. It is permissible because we are not
    // looking for the exact sequence of moves in the solution--only the number
    // of moves.
    std::string Hash() const
    {
        std::string retval(7, '@');

        // Make a letter for each MG pair (0..16) as @..P.
        // Sort the above, then add the elevator as suffix (0-3).
        for (auto index = 0U; index < 4; index++)
        {
            for (auto strpos = 0U; strpos < 7; strpos++)
            {
                unsigned short microchip = 1 << (strpos * 2);
                unsigned short generator = microchip << 1;

                if (IsObjectAtIndex(microchip, index))
                    retval[strpos] = index + ((retval[strpos] == '@') ? 'A' : retval[strpos]);
                if (IsObjectAtIndex(generator, index))
                    retval[strpos] = 4 * index + ((retval[strpos] == '@') ? 'A' : retval[strpos]);
            }
        }

        std::sort(retval.begin(), retval.end());
        retval.push_back('0' + GetElevatorIndex());

        return retval;
    }

private:
    union
    {
        unsigned short floor[4] = { 0, 0, 0, 0 };
        unsigned long long hash;
    };
};

struct QueueNode
{
    State state;
    unsigned numMoves;
};

std::set<std::string> seen;
std::queue<QueueNode> queue;
unsigned bestSolution = INT_MAX;

inline void TryMoveSecondObject(QueueNode node, unsigned short name, unsigned oldIndex, unsigned newIndex)
{
    node.state.MoveObject(name, oldIndex, newIndex);

    if (node.state.IsIndexValid(oldIndex) && node.state.IsIndexValid(newIndex))
        queue.push(node);
}

inline void TryMoveFirstObject(QueueNode node, unsigned short name, unsigned oldIndex, unsigned newIndex)
{
    node.state.MoveObject(name, oldIndex, newIndex);
    node.state.MoveObject(Elevator, oldIndex, newIndex);
    node.numMoves++;

    if (node.state.IsIndexValid(oldIndex) && node.state.IsIndexValid(newIndex))
        queue.push(node);

    for (unsigned short curr = name << 1; curr < Elevator; curr <<= 1)
        if (node.state.IsObjectAtIndex(curr, oldIndex))
            TryMoveSecondObject(node, curr, oldIndex, newIndex);
}

inline void TryMove(QueueNode &node, unsigned oldIndex, unsigned newIndex)
{
    for (unsigned short curr = 0x1; curr < Elevator; curr <<= 1)
        if (node.state.IsObjectAtIndex(curr, oldIndex))
            TryMoveFirstObject(node, curr, oldIndex, newIndex);
}

unsigned Solve(const State &initState)
{
    seen.clear();
    while (!queue.empty()) queue.pop();
    bestSolution = INT_MAX;

    queue.push(QueueNode{ initState, 0 });

    while (!queue.empty())
    {
        QueueNode node = queue.front();
        queue.pop();

        auto hash = node.state.Hash();
        if (seen.find(hash) != seen.end())
            continue;

        seen.insert(hash);

        if (node.state.IsSolved())
        {
            if (node.numMoves < bestSolution)
                bestSolution = node.numMoves;

            continue;
        }

        if (node.numMoves + 1 < bestSolution)
        {
            auto index = node.state.GetElevatorIndex();

            if (index != 3) TryMove(node, index, index + 1);
            if (index != 0) TryMove(node, index, index - 1);
        }
    }
    
    return bestSolution;
}

void DoCommonStateSetupForParts1And2(State &state)
{
    state.PlaceObjectAtFloor(Polonium_M, 2);
    state.PlaceObjectAtFloor(Promethium_M, 2);
    state.PlaceObjectAtFloor(Polonium_G, 1);
    state.PlaceObjectAtFloor(Thulium_G, 1);
    state.PlaceObjectAtFloor(Thulium_M, 1);
    state.PlaceObjectAtFloor(Promethium_G, 1);
    state.PlaceObjectAtFloor(Ruthenium_G, 1);
    state.PlaceObjectAtFloor(Ruthenium_M, 1);
    state.PlaceObjectAtFloor(Cobalt_G, 1);
    state.PlaceObjectAtFloor(Cobalt_M, 1);
    state.PlaceObjectAtFloor(Elevator, 1);
}

unsigned DoPart1()
{
    State state;
    DoCommonStateSetupForParts1And2(state);

    return Solve(state);
}

unsigned DoPart2()
{
    State state;
    DoCommonStateSetupForParts1And2(state);

    state.PlaceObjectAtFloor(Elerium_G, 1);
    state.PlaceObjectAtFloor(Elerium_M, 1);
    state.PlaceObjectAtFloor(Dilithium_G, 1);
    state.PlaceObjectAtFloor(Dilithium_M, 1);

    return Solve(state);
}

int main()
{
    double startTime = clock();

    auto part1 = DoPart1();
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 47);

    auto part2 = DoPart2();
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 71);

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;
    return 0;
}
