#include <iostream>
#include <sstream>
#include <cassert>

// Reverse engineering the input program, there are 14 nearly identical blocks,
// one for each digit of the 14-digit input. In each block, some processing
// happens and the only thing that matters at the end of the block is the value
// in z. The program is effectively implementing a stack data structure by
// manipulating the value of z, which it treats like a base 26 value.
//
// Each block contains one branch. The block will either: (1) right-shift the
// base 26 number in z (like a pop) or it will (2) left-shift the base 26 number
// and add a modified value to the rightmost part from the step's input (like a
// push).
//
// We need z to be zero at the end of all steps (i.e., empty stack) to represent
// a valid model number, so we want each step to pop if it is possible. Looking
// at the code for each step, in some cases, a particular value of the input
// digit will trigger the pop. In some cases, the required input would be
// outside the 1-9 range, so it's not possible to avoid a push.
//
// Value of z after each input step:
//  1: (w1+13)
//  2: (w1+13)*26   + (w2+10)
//  3: (w1+13)*26^2 + (w2+10)*26   + (w3+5)
//  4: (w1+13)*26   + (w2+10)
//  5: (w1+13)*26^2 + (w2+10)*26   + (w5+5)
//  6: (w1+13)*26   + (w2+10)
//  7: (w1+13)*26^2 + (w2+10)*26   + (w7+4)
//  8: (w1+13)*26^3 + (w2+10)*26^2 + (w7+4)*26   + (w8+11)
//  9: (w1+13)*26^4 + (w2+10)*26^3 + (w7+4)*26^2 + (w8+11)*26 + (w9+1)
// 10: (w1+13)*26^3 + (w2+10)*26^2 + (w7+4)*26   + (w8+11)
// 11: (w1+13)*26^2 + (w2+10)*26   + (w7+4)
// 12: (w1+13)*26   + (w2+10)
// 13: (w1+13)
// 14: 0
//
// Conditions for the steps where a reduction can be made (others not possible):
//  w4 = w3-6
//  w6 = w5+5
// w10 = w9-5
// w11 = w8+1
// w12 = w7-8
// w13 = w2+7
// w14 = w1+8
//
// A simple brute force against those criteria finds the answers almost
// immediately.

auto do_part(bool part2)
{
    auto start = part2 ? 1 : 9;
    auto past_end = part2 ? 10 : 0;
    auto inc_dec = part2 ? 1 : -1;
    auto retval = -1LL;

    for (auto w1 = start; w1 != past_end; w1 += inc_dec)
    {
        for (auto w2 = start; w2 != past_end; w2 += inc_dec)
        {
            for (auto w3 = start; w3 != past_end; w3 += inc_dec)
            {
                auto w4 = w3 - 6;
                if (w4 < 1 || w4 > 9) continue;

                for (auto w5 = start; w5 != past_end; w5 += inc_dec)
                {
                    auto w6 = w5 + 5;
                    if (w6 < 1 || w6 > 9) continue;

                    for (auto w7 = start; w7 != past_end; w7 += inc_dec)
                    {
                        for (auto w8 = start; w8 != past_end; w8 += inc_dec)
                        {
                            for (auto w9 = start; w9 != past_end; w9 += inc_dec)
                            {
                                auto w10 = w9 - 5;
                                if (w10 < 1 || w10 > 9) continue;

                                auto w11 = w8 + 1;
                                if (w11 < 1 || w11 > 9) continue;

                                auto w12 = w7 - 8;
                                if (w12 < 1 || w12 > 9) continue;

                                auto w13 = w2 + 7;
                                if (w13 < 1 || w13 > 9) continue;

                                auto w14 = w1 + 8;
                                if (w14 < 1 || w14 > 9) continue;

                                std::stringstream ss;
                                ss << w1 << w2 << w3 << w4 << w5 << w6 << w7 << w8 << w9 << w10 << w11 << w12 << w13 << w14;
                                ss >> retval;
                                return retval;
                            }
                        }
                    }
                }
            }
        }
    }

    return retval;
}

int main()
{
    auto part1 = do_part(false);  
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part(true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 12934998949199);
    assert(part2 == 11711691612189);
    return 0;
}
