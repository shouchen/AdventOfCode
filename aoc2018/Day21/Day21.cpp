#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <cassert>

struct Instruction
{
    std::string opcode;
    int a, b, c;
};

std::vector<Instruction> program;
int reg[6];

void do_instruction(const std::string opcode, int a, int b, int c)
{
    if (opcode == "addr")
        reg[c] = reg[a] + reg[b];
    else if (opcode == "addi")
        reg[c] = reg[a] + b;
    else if (opcode == "mulr")
        reg[c] = reg[a] * reg[b];
    else if (opcode == "muli")
        reg[c] = reg[a] * b;
    else if (opcode == "banr")
        reg[c] = reg[a] & reg[b];
    else if (opcode == "bani")
        reg[c] = reg[a] & b;
    else if (opcode == "borr")
        reg[c] = reg[a] | reg[b];
    else if (opcode == "bori")
        reg[c] = reg[a] | b;
    else if (opcode == "setr")
        reg[c] = reg[a];
    else if (opcode == "seti")
        reg[c] = a;
    else if (opcode == "gtir")
        reg[c] = (a > reg[b]) ? 1 : 0;
    else if (opcode == "gtri")
        reg[c] = (reg[a] > b) ? 1 : 0;
    else if (opcode == "gtrr")
        reg[c] = (reg[a] > reg[b]) ? 1 : 0;
    else if (opcode == "eqir")
        reg[c] = (a == reg[b]) ? 1 : 0;
    else if (opcode == "eqri")
        reg[c] = (reg[a] == b) ? 1 : 0;
    else if (opcode == "eqrr")
        reg[c] = (reg[a] == reg[b]) ? 1 : 0;
    else
        assert(false);
}


unsigned run_program_part1(int initial_r0, int ip_reg)
{
    int ip = 0;

    reg[0] = initial_r0;
    reg[1] = reg[2] = reg[3] = reg[4] = reg[5] = 0;

    for (;;)
    {
        reg[ip_reg] = ip;

        // By disassembly, shortest run will be when reg[0] and reg[3] have same value on first iteration.
        if (ip == 28)
            return reg[3]; // Part 1

        do_instruction(program[ip].opcode, program[ip].a, program[ip].b, program[ip].c);

        ip = reg[ip_reg];
        ip++;
        if (ip >= program.size())
            break;
    }
}

unsigned run_program_part2(int initial_r0, int ip_reg)
{
    std::set<int> seen;
    int previous = 0;

    int ip = 0;

    reg[0] = initial_r0;
    reg[1] = reg[2] = reg[3] = reg[4] = reg[5] = 0;

    for (;;)
    {
        reg[ip_reg] = ip;

        // By disassembly, longest will be the one before a repeat.
        if (ip == 28)
        {
            if (seen.find(reg[3]) != seen.end())
                return previous;
            previous = reg[3];
        }

        do_instruction(program[ip].opcode, program[ip].a, program[ip].b, program[ip].c);

        ip = reg[ip_reg];
        ip++;
        if (ip >= program.size())
            break;
    }
}

int do_part2()
{
    std::set<int> seen;
    int previous = 0;

    int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;

    do
    {
        c = d | 0x10000;
        d = 832312;

        // This for-loop manipulates d and c
        for (;;)
        {
            d = (d + (c & 0xff)) & 0xffffff; // add least significant byte of c to d, keep only lowest three bytes
            d = (d * 65899) & 0xffffff;      // multiply by big number and again keep lowest three bytes
            if (c < 256) break;              // exit when c is a single byte

            // This divides c by 256, which is right shift by 8.
            //b = 0;
            //for (;;)
            //{
            //    if (c < (b + 1) * 256) break;
            //    b++;
            //}
            // c = b;

            c >>= 8;
        }

        // When do we get a repeat?
        if (seen.find(d) != seen.end())
            return previous;
        seen.insert(d);
        previous = d;
    } while (d != a);
}

// 9258470 for Part 2 is right

int main()
{
#if 0
    int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;

    d = 0;
    do
    {
        b = c | 0x10000;
        d = 0xcb338;

        for (;;)
        {
            d += c & 0xff;
            d &= 0xffffff;
            d *= 0x1106b;
            d &= 0xffffff;
            if (c < 256) break;

            for (b = 0; e <= c; b++)
                e = (b + 1) * 256;

            c = b;
        }

    } while (d != a);
#endif

    std::ifstream file("input.txt");

    std::string pound_ip = "#ip";
    int ip_reg, ip = 0;
    file >> pound_ip >> ip_reg;

    std::string instruction;
    int a, b, c;
    while (file >> instruction >> a >> b >> c)
        program.push_back(Instruction{ instruction, a, b, c });

    auto part1 = run_program_part1(0, ip_reg);
    auto part2 = do_part2();

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 212115);
    assert(part2 == 9258470);

    return 0;
}
// 212115 was right!


/* ip is reg f
case 0: f = 0; d = 123;  // seti 123 0 3
case 1: f = 1; d = d & 0x1c8; // bani 3 456 3
case 2: f = 2; d = (d == 72) ? 1 : 0; // eqri 3 72 3
case 3: f = 3; f = d + f; // addr 3 5 5
case 4: f = 4; f = 0; // seti 0 0 5
case 5: f = 5; d = 0; // seti 0 5 3
case 6: f = 6; c = d | 0x10000; // bori 3 65536 2
case 7: f = 7; d = 832312; // seti 832312 1 3
case 8: f = 8; b = c & 0xff; // bani 2 255 1
case 9: f = 9; d = d + b; // addr 3 1 3
case 10: f = 10; d = d & 0xffffff; //bani 3 16777215 3
case 11: f = 11; d = d * 0x1106b; // muli 3 65899 3
case 12: f = 12; d = d & 0xffffff; // bani 3 16777215 3
case 13: f = 13; b = (256 > c) ? 1 : 0; // gtir 256 2 1
case 14: f = 14; f = b + f; // addr 1 5 5
case 15: f = 15; f = f + 1; // addi 5 1 5
case 16: f = 16; f = 27; // seti 27 7 5
case 17: f = 17; b = 0; // seti 0 2 1
case 18: f = 18; e = b + 1; // addi 1 1 4
case 19: f = 19; e = e * 256; //muli 4 256 4
case 20: f = 20; e = (e > c) ? 1 : 0; // gtrr 4 2 4
case 21: f = 21; f = e + f; // addr 4 5 5
case 22: f = 22; f = f + 1; // addi 5 1 5
case 23: f = 23; f = 25; // seti 25 1 5
case 24: f = 24; b = b + 1; // addi 1 1 1
case 25: f = 25; f = 17; // seti 17 0 5
case 26: f = 26; c = b; // setr 1 7 2
case 27: f = 27; f = 7; // seti 7 2 5
case 28: f = 28; b = (d == a) ? 1 : 0; // eqrr 3 0 1
case 29: f = 29; f = b + f; // addr 1 5 5
case 30: f = 30; f = 5; // seti 5 5 5
*/

/* ip is reg f
// NOP STUFF
case 0: d = 123;
case 1: d = d & 0x1c8;
case 2: d = (d == 72) ? 1 : 0;
case 3: goto d + 1;
case 4: goto 1;


case 5: d = 0;
case 6: c = d | 0x10000;
case 7: d = 832312;

case 8: b = c & 0xff;
case 9: d = d + b;
case 10: d = d & 0xffffff;
case 11: d = d * 0x1106b;
case 12: d = d & 0xffffff;
case 13:
case 14: if (256 > c) goto 28;

case 17: b = 0;
case 18: e = b + 1;
case 19: e = e * 256;
case 21: if (e > c) goto 26;
case 24: b++;
case 25: goto 18;

case 26: c = b;
case 27: goto 8;

case 28: 
case 29: if (d != a) goto 6;
*/

/* ip is reg f
// NOP STUFF
case 0: d = 123;
case 1: d = d & 0x1c8;
case 2: d = (d == 72) ? 1 : 0;
case 3: goto d + 1;
case 4: goto 1;

case 5: d = 0;
case 6: c = d | 0x10000;
case 7: d = 832312;

////////////////////

case 8: b = c & 0xff;
case 9: d = d + b;
case 10: d = d & 0xffffff;
case 11: d = d * 0x1106b;
case 12: d = d & 0xffffff;
case 13:
case 14: if (256 > c) goto 28;

for (b = 0; e <= c; b++)
  e = (b + 1) * 256;

case 26: c = b;
case 27: goto 8;

case 28:
case 29: if (d != a) goto 6;
*/

/*
// NOP STUFF

d = 0;

do
{
    c = d | 0x10000;
    d = 0x0cb338;

    for (;;)
    {
        d = (d + (c & 0xff)) & 0xffffff;
        d = (d * 0x1106b) & 0xffffff;
        if (c < 256) break;

        for (b = 0; e <= c; b++)
            e = (b + 1) * 256;

        c = b;
    }
} while (d != a);
*/

/* PART TWO

do
{
    c = d | 0x10000; // c takes 17 LSB f d
    d = 832312;

    for (;;)
    {
        d = d + (c & 0xff) & 0xffffff;
        d = (d * 65899) & 0xffffff;

        if (c < 256)
            break;

        for (b = 0; e <= c; b++)
            e = (b + 1) * 256;

        c = b;
    }
} while (d != a);
*/
