#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <strstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <numeric>
#include <algorithm>
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

#if 0
void Day19Part2()
{
    int ip = 0;
    int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;

    for (;;)
    {
        b = ip;

        switch (ip)
        {
        case 0: b += 16; break; //addi 1 16 1
        case 1: d = 1; break;   //seti 1 1 3
        case 2: f = 1; break;   //seti 1 9 5
        case 3: c = d * f; break;  //mulr 3 5 2
        case 4: c = (c == e) ? 1 : 0; break; //eqrr 2 4 2
        case 5: b = c + b; break; //addr 2 1 1
        case 6: b = b + 1; break; //addi 1 1 1
        case 7: a = d + a; break; //addr 3 0 0
        case 8: f = f + 1; break; //addi 5 1 5
        case 9: c = (f > e) ? 1 : 0; break; //gtrr 5 4 2
        case 10: b = b + c; break; // addr 1 2 1
        case 11: b = 2; break; // seti 2 6 1
        case 12: d = d + 1; break; // addi 3 1 3
        case 13: c = (d > e) ? 1 : 0; break; // gtrr 3 4 2
        case 14: b = c + b; break; // addr 2 1 1
        case 15: b = 1; break; // seti 1 6 1
        case 16: b = b * b; break; // mulr 1 1 1
        case 17: e = e + 2; break; // addi 4 2 4
        case 18: e = e * e; break; // mulr 4 4 4
        case 19: e = b * e; break; // mulr 1 4 4
        case 20: e = e * 11; break; // muli 4 11 4
        case 21: c = c + 6; break; // addi 2 6 2
        case 22: c = c * b; break; // mulr 2 1 2
        case 23: c = c + 2; break; // addi 2 2 2
        case 24: e = e + c; break; // addr 4 2 4
        case 25: b = b + a; break; // addr 1 0 1
        case 26: b = 0; break; // seti 0 3 1
        case 27: c = b; break; // setr 1 4 2
        case 28: c = c * b; break; // mulr 2 1 2
        case 29: c = b + c; break; // addr 1 2 2
        case 30: c = b * c; break; // mulr 1 2 2
        case 31: c = c * 14; break; // muli 2 14 2
        case 32: c = c * b; break; // mulr 2 1 2
        case 33: e = e + c; break; // addr 4 2 4
        case 34: a = 0; break; // seti 0 0 0
        case 35: b = 0; break; // seti 0 4 1
        default:
            assert(false);
        //    do_instruction(program[ip].opcode, program[ip].a, program[ip].b, program[ip].c);
        }
        ip = b + 1;
        if (ip < 0 || ip > 35) break;
    }

    std::cout << a << std::endl;
}

int Day19Part2()
{
    int ip = 0;
    int a = 0, c = 0, d = 0, e = 0, f = 0;

    for (;;)
    {
        switch (ip)
        {
        case 0: ip += 16; ip++; break; //addi 1 16 1
        case 1: d = 1; ip++; break;   //seti 1 1 3
        case 2: f = 1; ip++; break;   //seti 1 9 5
        case 3: c = d * f; ip++; break;  //mulr 3 5 2
        case 4: c = (c == e) ? 1 : 0; ip++; break; //eqrr 2 4 2
        case 5: ip += c; ip++; break; //addr 2 1 1
        case 6: ip++; ip++; break; //addi 1 1 1
        case 7: a += d; ip++; break; //addr 3 0 0
        case 8: f++; ip++; break; //addi 5 1 5
        case 9: c = (f > e) ? 1 : 0; ip++; break; //gtrr 5 4 2
        case 10: ip += c; ip++; break; // addr 1 2 1
        case 11: ip = 2; ip++; break; // seti 2 6 1
        case 12: d++; ip++; break; // addi 3 1 3
        case 13: c = (d > e) ? 1 : 0; ip++; break; // gtrr 3 4 2
        case 14: ip += c; ip++; break; // addr 2 1 1
        case 15: ip = 1; ip++; break; // seti 1 6 1
        case 16: ip *= ip; ip++; break; // mulr 1 1 1
        case 17: e += 2; ip++; break; // addi 4 2 4
        case 18: e *= e; ip++; break; // mulr 4 4 4
        case 19: e *= ip; ip++; break; // mulr 1 4 4
        case 20: e *= 11; ip++; break; // muli 4 11 4
        case 21: c += 6; ip++; break; // addi 2 6 2
        case 22: c *= ip; ip++; break; // mulr 2 1 2
        case 23: c += 2; ip++; break; // addi 2 2 2
        case 24: e += c; ip++; break; // addr 4 2 4
        case 25: ip += a; ip++; break; // addr 1 0 1
        case 26: ip = 0; ip++; break; // seti 0 3 1
        case 27: c = ip; ip++; break; // setr 1 4 2
        case 28: c *= ip; ip++; break; // mulr 2 1 2
        case 29: c += ip; ip++; break; // addr 1 2 2
        case 30: c *= ip; ip++; break; // mulr 1 2 2
        case 31: c *= 14; ip++; break; // muli 2 14 2
        case 32: c *= ip; ip++; break; // mulr 2 1 2
        case 33: e += c; ip++; break; // addr 4 2 4
        case 34: a = 0; ip++; break; // seti 0 0 0
        case 35: ip = 0; ip++; break; // seti 0 4 1
        default:
            std::cout << a << std::endl;
            return a;
        }
    }
}

int Day19Part2()
{
    int ip = 0;
    int a = 0, c = 0, d = 0, e = 0, f = 0;

    for (;;)
    {
        switch (ip)
        {
        case 0: ip = 17; break; //addi 1 16 1
        case 1: d = 1; ip = 2; //seti 1 1 3
        case 2: f = 1; ip = 3; break;   //seti 1 9 5
        case 3: c = d * f; ip = 4; break;  //mulr 3 5 2
        case 4: c = (c == e) ? 1 : 0; ip = 5; break; //eqrr 2 4 2
        case 5: ip = c + 6; break; //addr 2 1 1
        case 6: ip = 8; break; //addi 1 1 1
        case 7: a += d; ip = 8; break; //addr 3 0 0
        case 8: f++; ip = 9; break; //addi 5 1 5
        case 9: c = (f > e) ? 1 : 0; ip = 10; break; //gtrr 5 4 2
        case 10: ip = c + 11; break; // addr 1 2 1
        case 11: ip = 3; break; // seti 2 6 1
        case 12: d++; ip = 13; break; // addi 3 1 3
        case 13: c = (d > e) ? 1 : 0; ip = 14; break; // gtrr 3 4 2
        case 14: ip = c + 15; break; // addr 2 1 1
        case 15: ip = 2; break; // seti 1 6 1
        case 16: ip = 16 * ip + 1; break; // mulr 1 1 1
        case 17: e += 2; ip = 18; break; // addi 4 2 4
        case 18: e *= e; ip = 19; break; // mulr 4 4 4
        case 19: e *= 19; ip = 20; break; // mulr 1 4 4
        case 20: e *= 11; ip = 21; break; // muli 4 11 4
        case 21: c += 6; ip = 22; break; // addi 2 6 2
        case 22: c *= 22; ip = 23; break; // mulr 2 1 2
        case 23: c += 2; ip = 24; break; // addi 2 2 2
        case 24: e += c; ip = 25; break; // addr 4 2 4
        case 25: ip = a + 26; break; // addr 1 0 1
        case 26: ip = 1; break; // seti 0 3 1
        case 27: c = ip + 1; ip = 28;  break; // setr 1 4 2
        case 28: c *= 28; ip = 29; break; // mulr 2 1 2
        case 29: c += 29; ip = 30; break; // addr 1 2 2
        case 30: c *= 30; ip = 31; break; // mulr 1 2 2
        case 31: c *= 14; ip = 32; break; // muli 2 14 2
        case 32: c *= 32; ip = 33; break; // mulr 2 1 2
        case 33: e += c; ip = 34; break; // addr 4 2 4
        case 34: a = 0; ip = 35; break; // seti 0 0 0
        case 35: ip = 1; break; // seti 0 4 1
        default:
            std::cout << a << std::endl;
            return a;
        }
    }
}

int Day19Part2()
{
    int ip = 0;
    int a = 0, c = 0, d = 0, e = 0, f = 0;

    for (;;)
    {
        switch (ip)
        {
        case 0: ip = 17; break; //addi 1 16 1
        case 1: d = 1; //seti 1 1 3
        case 2: f = 1; //seti 1 9 5
        case 3: c = d * f; //mulr 3 5 2
        case 4: c = (c == e) ? 1 : 0; //eqrr 2 4 2
        case 5: ip = c + 6; break; //addr 2 1 1
        case 6: ip = 8; break; //addi 1 1 1
        case 7: a += d; //addr 3 0 0
        case 8: f++; //addi 5 1 5
        case 9: c = (f > e) ? 1 : 0; //gtrr 5 4 2
        case 10: ip = c + 11; break; // addr 1 2 1
        case 11: ip = 3; break; // seti 2 6 1
        case 12: d++; // addi 3 1 3
        case 13: c = (d > e) ? 1 : 0; // gtrr 3 4 2
        case 14: ip = c + 15; break; // addr 2 1 1
        case 15: ip = 2; break; // seti 1 6 1
        case 16: ip = 16 * 16 + 1; break; // mulr 1 1 1
        case 17: e += 2; // addi 4 2 4
        case 18: e *= e; // mulr 4 4 4
        case 19: e *= 19; // mulr 1 4 4
        case 20: e *= 11; // muli 4 11 4
        case 21: c += 6; // addi 2 6 2
        case 22: c *= 22; // mulr 2 1 2
        case 23: c += 2; // addi 2 2 2
        case 24: e += c; // addr 4 2 4
        case 25: ip = a + 26; break; // addr 1 0 1
        case 26: ip = 1; break; // seti 0 3 1
        case 27: c = ip + 1; // setr 1 4 2
        case 28: c *= 28; // mulr 2 1 2
        case 29: c += 29; // addr 1 2 2
        case 30: c *= 30; // mulr 1 2 2
        case 31: c *= 14; // muli 2 14 2
        case 32: c *= 32; // mulr 2 1 2
        case 33: e += c; // addr 4 2 4
        case 34: a = 0; // seti 0 0 0
        case 35: ip = 1; break; // seti 0 4 1
        default:
            std::cout << a << std::endl;
            return a;
        }
    }
}

int Day19Part2()
{
    int ip = 0;
    int a = 0, c = 0, d = 0, e = 0, f = 0;

    for (;;)
    {
        switch (ip)
        {
        case 0: ip = 17; break; //addi 1 16 1
        case 1: d = 1; //seti 1 1 3
        case 2: f = 1; //seti 1 9 5
        case 3: c = d * f; //mulr 3 5 2
        case 4: c = (c == e) ? 1 : 0; //eqrr 2 4 2
        case 5: ip = c + 6; break; //addr 2 1 1
        case 6: ip = 8; break; //addi 1 1 1
        case 7: a += d; //addr 3 0 0
        case 8: f++; //addi 5 1 5
        case 9: c = (f > e) ? 1 : 0; //gtrr 5 4 2
        case 10: ip = c + 11; break; // addr 1 2 1
        case 11: ip = 3; break; // seti 2 6 1
        case 12: d++; // addi 3 1 3
        case 13: c = (d > e) ? 1 : 0; // gtrr 3 4 2
        case 14: ip = c + 15; break; // addr 2 1 1
        case 15: ip = 2; break; // seti 1 6 1
        case 16: ip = 257; break; // mulr 1 1 1
        case 17: e += 2; // addi 4 2 4
        case 18: e *= e; // mulr 4 4 4
        case 19: e *= 19; // mulr 1 4 4
        case 20: e *= 11; // muli 4 11 4
        case 21: c += 6; // addi 2 6 2
        case 22: c *= 22; // mulr 2 1 2
        case 23: c += 2; // addi 2 2 2
        case 24: e += c; // addr 4 2 4
        case 25: ip = a + 26; break; // addr 1 0 1
        case 26: ip = 1; break; // seti 0 3 1
        case 27: c = 28; // setr 1 4 2
        case 28: c *= 28; // mulr 2 1 2
        case 29: c += 29; // addr 1 2 2
        case 30: c *= 30; // mulr 1 2 2
        case 31: c *= 14; // muli 2 14 2
        case 32: c *= 32; // mulr 2 1 2
        case 33: e += c; // addr 4 2 4
        case 34: a = 0; // seti 0 0 0
        case 35: ip = 1; break; // seti 0 4 1
        default:
            std::cout << a << std::endl;
            return a;
        }
    }
}

int Day19Part2()
{
    int ip = 0;
    int a = 0, c = 0, d = 0, e = 0, f = 0;

    for (;;)
    {
        switch (ip)
        {
        case 0: ip = 17; break; //addi 1 16 1              // This is a jump
        case 1: d = 1; //seti 1 1 3
        case 2: f = 1; //seti 1 9 5
        case 3: c = d * f; //mulr 3 5 2
        case 4: c = (c == e) ? 1 : 0; //eqrr 2 4 2
        case 5: ip = c + 6; break; //addr 2 1 1            // This is if-then-else with previous line
        case 6: ip = 8; break; //addi 1 1 1                // This is a jump
        case 7: a += d; //addr 3 0 0
        case 8: f++; //addi 5 1 5
        case 9: c = (f > e) ? 1 : 0; //gtrr 5 4 2
        case 10: ip = c + 11; break; // addr 1 2 1         // This is if-then-else with previous line
        case 11: ip = 3; break; // seti 2 6 1              // This is a jump
        case 12: d++; // addi 3 1 3
        case 13: c = (d > e) ? 1 : 0; // gtrr 3 4 2
        case 14: ip = c + 15; break; // addr 2 1 1         // This is if-then-else with previous line
        case 15: ip = 2; break; // seti 1 6 1              // This is a jump
        case 16: ip = 257; break; // mulr 1 1 1            // This is a jump
        case 17: e += 2; // addi 4 2 4
        case 18: e *= e; // mulr 4 4 4
        case 19: e *= 19; // mulr 1 4 4
        case 20: e *= 11; // muli 4 11 4
        case 21: c += 6; // addi 2 6 2
        case 22: c *= 22; // mulr 2 1 2
        case 23: c += 2; // addi 2 2 2
        case 24: e += c; // addr 4 2 4
        case 25: ip = a + 26; break; // addr 1 0 1         // This is a switch/jump table indexed by a?
        case 26: ip = 1; break; // seti 0 3 1              // This is a jump
        case 27: c = 28; // setr 1 4 2
        case 28: c *= 28; // mulr 2 1 2
        case 29: c += 29; // addr 1 2 2
        case 30: c *= 30; // mulr 1 2 2
        case 31: c *= 14; // muli 2 14 2
        case 32: c *= 32; // mulr 2 1 2
        case 33: e += c; // addr 4 2 4
        case 34: a = 0; // seti 0 0 0
        case 35: ip = 1; break; // seti 0 4 1              // This is a jump
        default:
            std::cout << a << std::endl;
            return a;
        }
    }
}

int Day19Part2()
{
    int a = 0, c = 0, d = 0, e = 0, f = 0;

    /*case0:*/ goto case17; //addi 1 16 1
case1: d = 1; //seti 1 1 3
case2: f = 1; //seti 1 9 5
case3: c = d * f; //mulr 3 5 2
    /*case4:*/ c = (c == e) ? 1 : 0; //eqrr 2 4 2
    /*case5:*/ if (c) goto case7; else goto case6;     //addr 2 1 1
case6: goto case8; //addi 1 1 1
case7: a += d; //addr 3 0 0
case8: f++; //addi 5 1 5
    /*case9:*/ c = (f > e) ? 1 : 0; //gtrr 5 4 2
    /*case10:*/ if (c) goto case12; else goto case11; // addr 1 2 1
case11: goto case3; // seti 2 6 1
case12: d++; // addi 3 1 3
    /*case13:*/ c = (d > e) ? 1 : 0; // gtrr 3 4 2
    /*case14:*/ if (c) goto case16; else goto case15; // addr 2 1 1
case15: goto case2; // seti 1 6 1
case16: goto case36; // mulr 1 1 1
case17: e += 2; // addi 4 2 4
    /*case18:*/ e *= e; // mulr 4 4 4
    /*case19:*/ e *= 19; // mulr 1 4 4
    /*case20:*/ e *= 11; // muli 4 11 4
    /*case21:*/ c += 6; // addi 2 6 2
    /*case22:*/ c *= 22; // mulr 2 1 2
    /*case23:*/ c += 2; // addi 2 2 2
    /*case24:*/ e += c; // addr 4 2 4
                        /*case25:*/ // addr 1 0 1
    if (a == 0) goto case26;
    else if (a == 1) goto case27;
    else if (a == 2) goto case28;
    else if (a == 3) goto case29;
    else if (a == 4) goto case30;
    else if (a == 5) goto case31;
    else if (a == 6) goto case32;
    else if (a == 7) goto case33;
    else if (a == 8) goto case34;
    else if (a == 9) goto case35;
    else goto case36;
case26: goto case1; // seti 0 3 1
case27: c = 28; // setr 1 4 2
case28: c *= 28; // mulr 2 1 2
case29: c += 29; // addr 1 2 2
case30: c *= 30; // mulr 1 2 2
case31: c *= 14; // muli 2 14 2
case32: c *= 32; // mulr 2 1 2
case33: e += c; // addr 4 2 4
case34: a = 0; // seti 0 0 0
case35: goto case1; // seti 0 4 1

case36:
    std::cout << a << std::endl;
    return a;
}

int Day19Part2()
{
    int a = 0, c = 0, d = 0, e = 0, f = 0;

    goto case17; //addi 1 16 1
case1:
    d = 1; //seti 1 1 3
case2:
    f = 1; //seti 1 9 5
case3:
    c = d * f; //mulr 3 5 2
    c = (c == e) ? 1 : 0; //eqrr 2 4 2
    if (c) goto case7; else goto case6;     //addr 2 1 1
case6:
    goto case8; //addi 1 1 1
case7:
    a += d; //addr 3 0 0
case8:
    f++; //addi 5 1 5
    c = (f > e) ? 1 : 0; //gtrr 5 4 2
    if (c) goto case12; else goto case11; // addr 1 2 1
case11:
    goto case3; // seti 2 6 1
case12:
    d++; // addi 3 1 3
    c = (d > e) ? 1 : 0; // gtrr 3 4 2
    if (c) goto case16; else goto case15; // addr 2 1 1
case15:
    goto case2; // seti 1 6 1
case16:
    goto case36; // mulr 1 1 1
case17:
    e += 2; // addi 4 2 4
    e *= e; // mulr 4 4 4
    e *= 19; // mulr 1 4 4
    e *= 11; // muli 4 11 4
    c += 6; // addi 2 6 2
    c *= 22; // mulr 2 1 2
    c += 2; // addi 2 2 2
    e += c; // addr 4 2 4
    // addr 1 0 1
        if (a == 0) goto case26;
        else if (a == 1) goto case27;
        else if (a == 2) goto case28;
        else if (a == 3) goto case29;
        else if (a == 4) goto case30;
        else if (a == 5) goto case31;
        else if (a == 6) goto case32;
        else if (a == 7) goto case33;
        else if (a == 8) goto case34;
        else if (a == 9) goto case35;
        else goto case36;
case26:
    goto case1; // seti 0 3 1
case27:
    c = 28; // setr 1 4 2
case28:
    c *= 28; // mulr 2 1 2
case29:
    c += 29; // addr 1 2 2
case30:
    c *= 30; // mulr 1 2 2
case31:
    c *= 14; // muli 2 14 2
case32:
    c *= 32; // mulr 2 1 2
case33:
    e += c; // addr 4 2 4
case34:
    a = 0; // seti 0 0 0
case35:
    goto case1; // seti 0 4 1

case36:
    std::cout << a << std::endl;
    return a;
}

int Day19Part2()
{
    int a = 0, c = 0, d = 0, e = 0, f = 0;

    goto case17;

    for (;;)
    {
        d = 1;
        do
        {
            f = 1;
            do
            {
                if (d * f == e)
                    a += d;
            } while (++f <= e);
        } while (++d <= e);
        goto exit;

    case17:
        e += 2;
        e *= e;
        e *= 19;
        e *= 11;
        c += 6;
        c *= 22;
        c += 2;
        e += c;
        if (a == 0) continue;
        else if (a == 1) goto case27;
        else if (a == 2) goto case28;
        else if (a == 3) goto case29;
        else if (a == 4) goto case30;
        else if (a == 5) goto case31;
        else if (a == 6) goto case32;
        else if (a == 7) goto case33;
        else if (a == 8) goto case34;
        else if (a == 9) goto case35;
        else goto exit;
    case27:
        c = 28;
    case28:
        c *= 28;
    case29:
        c += 29;
    case30:
        c *= 30;
    case31:
        c *= 14;
    case32:
        c *= 32;
    case33:
        e += c;
    case34:
        a = 0;
    case35:
        a;
    }

exit:
    std::cout << a << std::endl;
    return a;
}

int Day19Part2()
{
    int a = 0, c = 0, d = 0, e = 0, f = 0;

    goto start;

    for (;;)
    {
        d = 1;
        do
        {
            f = 1;
            do
            {
                if (d * f == e)
                    a += d;
            } while (++f <= e);
        } while (++d <= e);
        goto exit;

    start:
        c = (c + 6) * 22 + 2;
        e = ((e + 2) * (e + 2) * 19 * 11) + c;
        if (a == 0) continue;
        else if (a == 1) goto case1;
        else if (a == 2) goto case2;
        else if (a == 3) goto case3;
        else if (a == 4) goto case4;
        else if (a == 5) goto case5;
        else if (a == 6) goto case6;
        else if (a == 7) goto case7;
        else if (a == 8) goto case8;
        else goto exit;
    case1:
        c = 28;
    case2:
        c *= 28;
    case3:
        c += 29;
    case4:
        c *= 30;
    case5:
        c *= 14;
    case6:
        c *= 32;
    case7:
        e += c;
    case8:
        a = 0;
    }

exit:
    std::cout << a << std::endl;
    return a;
}

int Day19Part2()
{
    int a = 0, c = 0, d = 0, e = 0, f = 0;

    c = (c + 6) * 22 + 2;
    e = ((e + 2) * (e + 2) * 19 * 11) + c;

    if (a)
    {
        if (a == 1) goto case1;
        else if (a == 2) goto case2;
        else if (a == 3) goto case3;
        else if (a == 4) goto case4;
        else if (a == 5) goto case5;
        else if (a == 6) goto case6;
        else if (a == 7) goto case7;
        else if (a == 8) goto case8;
case1:
        c = 28;
case2:
        c *= 28;
case3:
        c += 29;
case4:
        c *= 30;
case5:
        c *= 14;
case6:
        c *= 32;
case7:
        e += c;
case8:
        a = 0;
    }

    d = 1;
    do
    {
        f = 1;
        do
        {
            if (d * f == e)
                a += d;
        } while (++f <= e);
    } while (++d <= e);

    std::cout << a << std::endl;
    return a;
}

int Day19Part2()
{
    int a = 1, c = 0, d = 0, e = 0, f = 0;

    c = (c + 6) * 22 + 2;
    e = ((e + 2) * (e + 2) * 19 * 11) + c;

    switch (a)
    {
    case 1: c = 28;
    case 2: c *= 28;
    case 3: c += 29;
    case 4: c *= 30;
    case 5: c *= 14;
    case 6: c *= 32;
    case 7: e += c;
    case 8: a = 0;
    }

    // Code below looks at large number e (part 1 is 970, part 2 is 10927690)
    // and adds up all the factors in a very slow way.
    for (d = 1; d <= e; d++)
        for (f = 1; f <= e; f++)
            if (d * f == e)
                a += d;

    std::cout << a << std::endl;
    return a;
}

WRONG
int Day19Part2()
{
    int a = 1, c = 0, d = 0, e = 0, f = 0;

    c = (c + 6) * 22 + 2;
    e = ((e + 2) * (e + 2) * 19 * 11) + c;

    switch (a)
    {
    case 1: c = 28;
    case 2: c *= 28;
    case 3: c += 29;
    case 4: c *= 30;
    case 5: c *= 14;
    case 6: c *= 32;
    case 7: e += c;
    case 8: a = 0;
    }

    // Code below looks at large number e (part 1 is 970, part 2 is 10927690)
    // and adds up all its factors in a very slow way.
    //for (d = 1; d <= e; d++)
    //    for (f = 1; f <= e; f++)
    //        if (d * f == e)
    //            a += d;

    // More efficient version of same code using mod
    for (d = 1; d <= e; d++)
        if (e % d == 0)
            a += d;

    std::cout << a << std::endl;
    return a;
} // 19766880 was too high
#endif

int Day19Part2(int initial_value_for_a)
{
    int ip = 0;
    int a = initial_value_for_a, b = 0, c = 0, d = 0, e = 0, f = 0;

    for (;;)
    {
        b = ip;

        switch (ip)
        {
        case 0: b += 16; break; //addi 1 16 1
        case 1: 
            for (d = 1; d <= e; d++)
                if (e % d == 0)
                    a += d;
            b = 16;
            break;

            //d = 1; break;   //seti 1 1 3
        case 2: f = 1; break;   //seti 1 9 5
        case 3: c = d * f; break;  //mulr 3 5 2
        case 4: c = (c == e) ? 1 : 0; break; //eqrr 2 4 2
        case 5: b = c + b; break; //addr 2 1 1
        case 6: b = b + 1; break; //addi 1 1 1
        case 7: a = d + a; break; //addr 3 0 0
        case 8: f = f + 1; break; //addi 5 1 5
        case 9: c = (f > e) ? 1 : 0; break; //gtrr 5 4 2
        case 10: b = b + c; break; // addr 1 2 1
        case 11: b = 2; break; // seti 2 6 1
        case 12: d = d + 1; break; // addi 3 1 3
        case 13: c = (d > e) ? 1 : 0; break; // gtrr 3 4 2
        case 14: b = c + b; break; // addr 2 1 1
        case 15: b = 1; break; // seti 1 6 1
        case 16: b = b * b; break; // mulr 1 1 1

        case 17: e = e + 2; break; // addi 4 2 4
        case 18: e = e * e; break; // mulr 4 4 4
        case 19: e = b * e; break; // mulr 1 4 4
        case 20: e = e * 11; break; // muli 4 11 4
        case 21: c = c + 6; break; // addi 2 6 2
        case 22: c = c * b; break; // mulr 2 1 2
        case 23: c = c + 2; break; // addi 2 2 2
        case 24: e = e + c; break; // addr 4 2 4
        case 25: b = b + a; break; // addr 1 0 1
        case 26: b = 0; break; // seti 0 3 1
        case 27: c = b; break; // setr 1 4 2
        case 28: c = c * b; break; // mulr 2 1 2
        case 29: c = b + c; break; // addr 1 2 2
        case 30: c = b * c; break; // mulr 1 2 2
        case 31: c = c * 14; break; // muli 2 14 2
        case 32: c = c * b; break; // mulr 2 1 2
        case 33: e = e + c; break; // addr 4 2 4
        case 34: a = 0; break; // seti 0 0 0
        case 35: b = 0; break; // seti 0 4 1
        default:
            assert(false);
            //    do_instruction(program[ip].opcode, program[ip].a, program[ip].b, program[ip].c);
        }
        ip = b + 1;
        if (ip < 0 || ip > 35) break;
    }

    //std::cout << a << std::endl;
    return a;
}
// 18992484 is right for part 2

int main()
{
    std::ifstream file("input.txt");

    std::string pound_ip = "#ip";
    int ip_reg, ip = 0;
    file >> pound_ip >> ip_reg;

    std::string instruction;
    int a, b, c;
    while (file >> instruction >> a >> b >> c)
        program.push_back(Instruction{ instruction, a, b, c });

    reg[0] = reg[1] = reg[2] = reg[3] = reg[4] = reg[5] = 0;

#if 0
    for (;;)
    {
        reg[ip_reg] = ip;
        do_instruction(program[ip].opcode, program[ip].a, program[ip].b, program[ip].c);
        ip = reg[ip_reg];
        ip++;
        if (ip >= program.size())
            break;
    }

    auto part1 = reg[0];
    std::cout << "Part 1: " << part1 << std::endl;
#endif
    auto part1 = Day19Part2(0);
    auto part2 = Day19Part2(1);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 1764);
    assert(part2 == 18992484);
    return 0;
}
// 1764 right part 1