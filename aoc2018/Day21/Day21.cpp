#include "stdafx.h"
#include <iostream>
#include <set>
#include <cassert>

auto do_part1()
{
    auto r0 = 0, r2 = 0, r3 = 0;

    do
    {
        r2 = r3 | 0x10000;
        r3 = 832312;

        for (;;)
        {
            r3 = (r3 + (r2 & 0xff)) & 0xffffff;
            r3 = (r3 * 65899) & 0xffffff;
            if (r2 < 256) break;

            r2 >>= 8;
        }

        // Ending after 1 iteration is best case, return that.
        return r3;
    } while (r3 != r0);
}

auto do_part2()
{
    std::set<int> seen;
    auto previous = -1;

    auto r0 = 0, r2 = 0, r3 = 0;

    do
    {
        r2 = r3 | 0x10000;
        r3 = 832312;

        for (;;)
        {
            r3 = (r3 + (r2 & 0xff)) & 0xffffff;
            r3 = (r3 * 65899) & 0xffffff;
            if (r2 < 256) break;

            r2 >>= 8;
        }

        // When do we get a repeat?
        if (seen.find(r3) != seen.end())
            break;
        seen.insert(r3);
        previous = r3;
    } while (r3 != r0);

    return previous;
}

int main()
{
    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 212115);
    assert(part2 == 9258470);

    return 0;
}
