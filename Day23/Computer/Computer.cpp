// Computer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

using Program = std::vector<std::string>;

class Computer
{
public:
    Computer(unsigned initialA = 0) : a(initialA) {}

    void ExecuteProgram(const Program &program)
    {
        while (pc < program.size())
        {
            std::string instruction = program[pc];

            if (strncmp(instruction.c_str(), "hlf", 3) == 0)
                DoHlf((instruction[4] == 'a') ? a : b);
            else if (strncmp(instruction.c_str(), "tpl", 3) == 0)
                DoTpl((instruction[4] == 'a') ? a : b);
            else if (strncmp(instruction.c_str(), "inc", 3) == 0)
                DoInc((instruction[4] == 'a') ? a : b);
            else if (strncmp(instruction.c_str(), "jmp", 3) == 0)
                DoJmp(atoi(&instruction[4]));
            else if (strncmp(instruction.c_str(), "jie", 3) == 0)
                DoJie((instruction[4] == 'a') ? a : b, atoi(&instruction[7]));
            else if (strncmp(instruction.c_str(), "jio", 3) == 0)
                DoJio((instruction[4] == 'a') ? a : b, atoi(&instruction[7]));
        }
    }

    unsigned GetB() { return b; }

private:
    void DoHlf(unsigned &r) { r /= 2; pc++; }
    void DoTpl(unsigned &r) { r *= 3; pc++; }
    void DoInc(unsigned &r) { r++; pc++; }
    void DoJmp(int offset) { pc += offset; }
    void DoJie(unsigned &r, int offset)  { pc += (r & 1) ? 1 : offset; }
    void DoJio(unsigned &r, int offset) { pc += (r == 1) ? offset : 1; }

    unsigned a = 0, b = 0;
    unsigned pc = 0;
};

void ReadProgram(const std::string &filename, Program &program)
{
    std::ifstream f(filename);
    for (std::string line; std::getline(f, line);)
        program.push_back(line);
}

void _tmain(int argc, _TCHAR *argv[])
{
    Program program;
    ReadProgram("Input.txt", program);

    Computer computer1;
    computer1.ExecuteProgram(program);
    unsigned b1 = computer1.GetB();
    std::cout << "part one: " << b1 << std::endl;

    Computer computer2(1);
    computer2.ExecuteProgram(program);
    unsigned b2 = computer2.GetB();
    std::cout << "part two: " << b2 << std::endl;

    assert(b1 == 255);
    assert(b2 == 334);
}
