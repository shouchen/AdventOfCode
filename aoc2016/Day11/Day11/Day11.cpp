#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <set>
#include <queue>
#include <ctime>
#include <cassert>

// These must be ordered by microchip-generator pairs (microchip followed by
// its corresponding generator). The algorithm and code below expect this.
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

const auto MaxObjects = 7U;
const auto NumFloors = 4U;

struct State
{
    inline unsigned GetElevatorIndex() const 
    {
        for (auto f = 0U; f < NumFloors; f++)
            if (floor[f] & Elevator)
                return f;
        return UINT_MAX;
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
        for (auto f = 0U; f < NumFloors - 1; f++)
            if (floor[f])
                return false;
        return true;
    }

    inline bool IsValidAtIndex(unsigned index)
    {
        auto generators = floor[index] & AllGenerators;
        if (!generators)
            return true;

        auto microchips = floor[index] & AllMicrochips;
        auto microchipsWithoutTheirGenerator = microchips & ~(generators >> 1);

        return !microchipsWithoutTheirGenerator;
    }

    // Special hash function that hashes to the same value all states that are
    // permutations of each other (but which are otherwise unique), which
    // dramatically prunes the search tree. This is permissible because we are
    // not looking for the exact sequence of moves in the solution--only the
    // number of moves (which would be the same for any congruent permutation).
    //
    // The hash returned is the floor of each of the objects (using 4 bits
    // apiece), followed by the elevator position in the remaining 8 bits. If an
    // object isn't on any floor (as for part 1), the floor is zero.
    unsigned long long Hash() const
    {
        std::vector<unsigned char> retval(MaxObjects + 1);
        retval[7] = GetElevatorIndex();

        for (auto index = 0U; index < NumFloors; index++)
        {
            for (auto strpos = 0U; strpos < MaxObjects; strpos++)
            {
                auto microchip = 1 << (strpos * 2);
                auto generator = microchip << 1;

                if (IsObjectAtIndex(microchip, index)) retval[strpos] += index + 1;
                if (IsObjectAtIndex(generator, index)) retval[strpos] += (index + 1) << 4;
            }
        }

        std::sort(retval.begin(), retval.end() - 1);
        return *reinterpret_cast<unsigned long long *>(retval.data());
    }

private:
    unsigned short floor[NumFloors] = { 0, 0, 0, 0 };
};

struct QueueNode
{
    State state;
    unsigned numMoves;
};

std::set<unsigned long long> seen;
std::queue<QueueNode> queue;
unsigned bestSolution = INT_MAX;

inline void TryMoveSecondObject(QueueNode node, unsigned short name, unsigned oldIndex, unsigned newIndex)
{
    node.state.MoveObject(name, oldIndex, newIndex);

    if (node.state.IsValidAtIndex(oldIndex) && node.state.IsValidAtIndex(newIndex))
        queue.push(node);
}

inline void TryMoveFirstObject(QueueNode node, unsigned short name, unsigned oldIndex, unsigned newIndex)
{
    node.state.MoveObject(name, oldIndex, newIndex);
    node.state.MoveObject(Elevator, oldIndex, newIndex);
    node.numMoves++;

    if (node.state.IsValidAtIndex(oldIndex) && node.state.IsValidAtIndex(newIndex))
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
    auto part2 = DoPart2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;

    assert(part1 == 47);
    assert(part2 == 71);
    return 0;
}
