// Computer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
Little Jane Marie just got her very first computer for Christmas from some unknown benefactor.It comes with instructions
and an example program, but the computer itself seems to be malfunctioning.She's curious what the program does, and would
like you to help her run it.

The manual explains that the computer supports two registers and six instructions(truly, it goes on to remind the reader,
a state - of - the - art technology).The registers are named a and b, can hold any non - negative integer, and begin with
a value of 0. The instructions are as follows :
- hlf r sets register r to half its current value, then continues with the next instruction.
- tpl r sets register r to triple its current value, then continues with the next instruction.
- inc r increments register r, adding 1 to it, then continues with the next instruction.
- jmp offset is a jump; it continues with the instruction offset away relative to itself.
- jie r, offset is like jmp, but only jumps if register r is even("jump if even").
- jio r, offset is like jmp, but only jumps if register r is 1 ("jump if one", not odd).

All three jump instructions work with an offset relative to that instruction.The offset is always written with a prefix
+ or - to indicate the direction of the jump(forward or backward, respectively).For example, jmp + 1 would simply continue
with the next instruction, while jmp + 0 would continuously jump back to itself forever.

The program exits when it tries to run an instruction beyond the ones defined.

For example, this program sets a to 2, because the jio instruction causes it to skip the tpl instruction :
inc a
jio a, +2
tpl a
inc a

What is the value in register b when the program in your puzzle input is finished executing ?
*/

unsigned a = 1, b = 0;
unsigned pc = 0;

// -hlf r sets register r to half its current value, then continues with the next instruction.
void DoHlf(unsigned &r)
{
    r /= 2;
    pc++;
}

// -tpl r sets register r to triple its current value, then continues with the next instruction.
void DoTpl(unsigned &r)
{
    r *= 3;
    pc++;
}

// -inc r increments register r, adding 1 to it, then continues with the next instruction.
void DoInc(unsigned &r)
{
    r++;
    pc++;
}

// - jmp offset is a jump; it continues with the instruction offset away relative to itself.
void DoJmp(int offset)
{
    pc += offset;
}

// - jie r, offset is like jmp, but only jumps if register r is even("jump if even").
void DoJie(unsigned &r, int offset)
{
    if (r & 1)
        pc++;
    else
        pc += offset;
}

// - jio r, offset is like jmp, but only jumps if register r is 1 ("jump if one", not odd).
void DoJio(unsigned &r, int offset)
{
    if (r == 1)
        pc += offset;
    else
        pc++;
}

void ExecuteInstruction(const string &instruction)
{
    if (strncmp(instruction.c_str(), "hlf", 3) == 0)
    {
        DoHlf((instruction[4] == 'a') ? a : b);
    }
    else if (strncmp(instruction.c_str(), "tpl", 3) == 0)
    {
        DoTpl((instruction[4] == 'a') ? a : b);
    }
    else if (strncmp(instruction.c_str(), "inc", 3) == 0)
    {
        DoInc((instruction[4] == 'a') ? a : b);
    }
    else if (strncmp(instruction.c_str(), "jmp", 3) == 0)
    {
        DoJmp(atoi(&instruction[4]));
    }
    else if (strncmp(instruction.c_str(), "jie", 3) == 0)
    {
        DoJie((instruction[4] == 'a') ? a : b, atoi(&instruction[7]));
    }
    else if (strncmp(instruction.c_str(), "jio", 3) == 0)
    {
        DoJio((instruction[4] == 'a') ? a : b, atoi(&instruction[7]));
    }
}

void ExecuteProgram(const vector<string> &program)
{
    while (pc < program.size())
    {
        string instruction = program[pc];
        ExecuteInstruction(instruction);
    }
}

void _tmain(int argc, _TCHAR *argv[])
{
    vector<string> program;

    ifstream f("Input.txt");
    for (string line; getline(f, line);)
        program.push_back(line);

    ExecuteProgram(program); // 255 FOR PART1
}
