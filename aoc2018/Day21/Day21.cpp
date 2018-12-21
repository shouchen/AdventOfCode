#include "stdafx.h"
#include <iostream>
#include <set>
#include <cassert>

// Reverse-engineering the input code, it is doing the equivalent of this:
//
//   do
//   {
//       r2 = r3 | 0x10000;
//       r3 = 832312;
//
//       for (;;)
//       {
//           r3 = (r3 + (r2 & 0xff)) & 0xffffff;
//           r3 = (r3 * 65899) & 0xffffff;
//           if (r2 < 256)
//               break;
//
//           r2 >>= 8;
//       }
//   } while (r3 != r0);
//
// The key is that r0 is only ever read from as a condition to exit the program;
// its value is never modified. The outer loop cycles through a set of 3-byte
// values in r3 until r3 matches r0. To find the least number of instructions to
// make the program exit (part 1), choose the value for r0 that makes the outer
// loop run only a single time. For part2, choose the value for r0 that makes
// the outer loop run the most times before the r3 cycle repeats itself.

void do_both_parts(int &part1, int &part2)
{
    std::set<int> r3_seen;
    auto previous_iteration_r3 = 0;

    auto r2 = 0, r3 = 0;

    for (;;)
    {
        r2 = r3 | 0x10000;
        r3 = 832312;

        for (;;)
        {
            r3 = (r3 + (r2 & 0xff)) & 0xffffff;
            r3 = (r3 * 65899) & 0xffffff;
            if (r2 < 256)
                break;

            r2 >>= 8;
        }

        if (r3_seen.empty())
        {
            part1 = r3;
        }
        else if (r3_seen.find(r3) != r3_seen.end())
        {
            part2 = previous_iteration_r3;
            break;
        }

        r3_seen.insert(previous_iteration_r3 = r3);
    }
}

auto main()
{
    auto part1 = 0, part2 = 0;
    do_both_parts(part1, part2);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 212115);
    assert(part2 == 9258470);

    return 0;
}
