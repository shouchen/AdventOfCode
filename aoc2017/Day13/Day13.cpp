#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

struct Data
{
    unsigned depth;
    unsigned range;
    unsigned security;
    bool down;
};

std::vector<Data> data;

void advance_security_item(Data &item)
{
    if (item.range == 1) return; // never moves

    if (item.down)
    {
        if (item.security < item.range - 1)
        {
            item.security++;
        }
        else
        {
            item.security--;
            item.down = false;
        }
    }
    else // up
    {
        if (item.security > 0)
        {
            item.security--;
        }
        else
        {
            item.security++;
            item.down = true;
        }
    }
}


void advance_security()
{
    for (auto &item : data)
    {
        advance_security_item(item);
    }
}

unsigned get_severity(unsigned depth)
{
    for (auto item : data)
        if (item.depth == depth)
        {
            return (item.security == 0) ? (item.depth  * item.range) : 0;
        }
    return 0;
}

bool caught(unsigned depth)
{
    for (auto item : data)
        if (item.depth == depth)
        {
            return item.security == 0;
        }
    return false;
}

int main()
{
    auto part1 = 0U, part2 = 0U;

    unsigned max_depth = 0;
    unsigned severity = 0;

    {
        std::ifstream f("input.txt");
        unsigned depth, range;
        char colon;
        while (f >> depth >> colon >> range)
        {
            // std::cout << depth << " " << range << std::endl;
            Data newData;
            newData.depth = depth;
            newData.range = range;
            newData.security = 0;
            newData.down = true;
            data.push_back(newData);

            if (depth > max_depth) max_depth = depth;
        }
    }

    // packet goes across
    for (auto depth = 0; depth <= max_depth; depth++)
    {
        severity += get_severity(depth);
        advance_security();
    }

    part1 = severity;

    ///////////////////// PART2
    {
        data.clear();
        {
            std::ifstream f("input.txt");
            unsigned depth, range;
            char colon;
            while (f >> depth >> colon >> range)
            {
                // std::cout << depth << " " << range << std::endl;
                Data newData;
                newData.depth = depth;
                newData.range = range;
                newData.security = 0;
                newData.down = true;
                data.push_back(newData);

                if (depth > max_depth) max_depth = depth;
            }
        }

        for (unsigned delay = 0; delay < UINT_MAX; delay++)
        {
            bool caught = false;
            for (auto item : data)
            {
                auto cycle = item.range * 2 - 2;
                if ((item.depth + delay) % cycle == 0)
                {
                    caught = true;
                    break;
                }               
            }

            if (!caught)
            {
                part2 = delay;
                break;
            }
        }
    }


    //assert(do_part1(5, "input-test.txt") == 12);

    //auto part1 = do_part1(256, "input.txt");
    //auto part2 = do_part2(256, "input.txt");

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 2384);
    assert(part2 == 3921270);
    return 0;
}
