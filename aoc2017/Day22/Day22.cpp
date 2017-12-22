#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

struct Coords
{
    int x, y;

    bool operator<(const Coords &rhs) const
    {
        if (y < rhs.y) return true;
        if (y > rhs.y) return false;
        return x < rhs.x;
    }
};

enum State { Clean, Weakened, Infected, Flagged };

unsigned do_part1(const std::string &filename, unsigned times)
{
    std::set<Coords> infected;

    std::ifstream f(filename);
    std::string line;

    int x = 0, y = 0;
    while (getline(f, line))
    {
        x = 0;
        for (auto c : line)
        {
            if (c == '#')
                infected.insert({ x, y });
            x++;
        }
        y++;
    }

    Coords curr = { x / 2, y / 2 };
    Coords dir = { 0, -1 };

    unsigned part1 = 0U;

    for (unsigned i = 0U; i < times; i++)
    {
        // Step 1. Turn right (if infected) or left (if clean).
        bool curr_infected = infected.find(curr) != infected.end();
        if (curr_infected)
        {
            if (dir.x == 0 && dir.y == -1)
                dir = { 1, 0 };     // up -> right
            else if (dir.x == 0 && dir.y == 1)
                dir = { -1, 0 };  // down -> left
            else if (dir.x == -1 && dir.y == 0)
                dir = { 0, -1 }; // left -> up
            else if (dir.x == 1 && dir.y == 0)
                dir = { 0, 1 }; // right -> down
        }
        else
        {
            if (dir.x == 0 && dir.y == -1) dir = { -1, 0 };     // up -> left
            else if (dir.x == 0 && dir.y == 1) dir = { 1, 0 };  // down -> right
            else if (dir.x == -1 && dir.y == 0) dir = { 0, 1 }; // left -> down
            else if (dir.x == 1 && dir.y == 0) dir = { 0, -1 }; // right -> up
        }

        // Step 2. Toggle current node.
        if (curr_infected)
        {
            infected.erase(curr);
        }
        else
        {
            infected.insert(curr);
            part1++;
        }

        // Step 3. Move forward
        curr.x += dir.x;
        curr.y += dir.y;
    }

    return part1;
}

unsigned do_part2(const std::string &filename, unsigned times)
{
    std::map<Coords, State> state;

    std::ifstream f(filename);
    std::string line;

    int x = 0, y = 0;
    while (getline(f, line))
    {
        x = 0;
        for (auto c : line)
        {
            if (c == '#')
                state[{ x, y }] = Infected;
            x++;
        }
        y++;
    }

    Coords curr = { x / 2, y / 2 };
    Coords dir = { 0, -1 };

    unsigned part2 = 0U;

    for (unsigned i = 0U; i < times; i++)
    {
        // Step 1. Turn left if clean, right if infected, 180 if flagged.
        State curr_state = Clean;
        auto found = state.find(curr);
        if (found != state.end())
            curr_state = found->second;

        switch (curr_state)
        {
        case Clean:
            if (dir.x == 0 && dir.y == -1)
                dir = { -1, 0 }; // up -> left
            else if (dir.x == 0 && dir.y == 1)
                dir = { 1, 0 };  // down -> right
            else if (dir.x == -1 && dir.y == 0)
                dir = { 0, 1 };  // left -> down
            else if (dir.x == 1 && dir.y == 0)
                dir = { 0, -1 }; // right -> up
            break;
        case Weakened:
            break;
        case Infected:
            if (dir.x == 0 && dir.y == -1)
                dir = { 1, 0 };  // up -> right
            else if (dir.x == 0 && dir.y == 1)
                dir = { -1, 0 }; // down -> left
            else if (dir.x == -1 && dir.y == 0)
                dir = { 0, -1 }; // left -> up
            else if (dir.x == 1 && dir.y == 0)
                dir = { 0, 1 };  // right -> down
            break;
        case Flagged:
            if (dir.x == 0 && dir.y == -1)
                dir = { 0, 1 };  // up -> down
            else if (dir.x == 0 && dir.y == 1)
                dir = { 0, -1 }; // down -> up
            else if (dir.x == -1 && dir.y == 0)
                dir = { 1, 0 };  // left -> right
            else if (dir.x == 1 && dir.y == 0)
                dir = { -1, 0 }; // right -> left
            break;
        }

        // Step 2. Toggle current node.
        switch (curr_state)
        {
        case Clean:
            curr_state = Weakened;
            state[curr] = curr_state;
            break;
        case Weakened:
            curr_state = Infected;
            state[curr] = curr_state;
            part2++;
            break;
        case Infected:
            curr_state = Flagged;
            state[curr] = curr_state;
            break;
        case Flagged:
            curr_state = Clean;
            state.erase(curr);
            break;
        }

        // Step 3. Move forward
        curr.x += dir.x;
        curr.y += dir.y;
    }

    return part2;
}

int main()
{
    assert(do_part1("input-test.txt", 7) == 5);
    assert(do_part1("input-test.txt", 70) == 41);
    assert(do_part1("input-test.txt", 10000) == 5587);

    assert(do_part2("input-test.txt", 100) == 26);
    assert(do_part2("input-test.txt", 10000000) == 2511944);

    auto part1 = do_part1("input.txt", 10000);
    auto part2 = do_part2("input.txt", 10000000);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 5246);
    assert(part2 == 2512059);
    return 0;
}
