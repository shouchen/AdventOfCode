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
    Computer(int initialC = 0) : a(0), b(0), c(initialC), d(0) {}

    void ExecuteProgram(const Program &program)
    {
        while (pc < program.size())
        {
            std::string instruction = program[pc];
            std::string::iterator pos = instruction.begin();

            if (strncmp(instruction.c_str(), "cpy ", 4) == 0)
            {
                int value = 0;
                pos += 4;
                if (*pos >= 'a' && *pos <= 'd')
                {
                    value = GetRegister(*pos++);
                }
                else
                {
                    value = atoi(&*pos);
                    while (*pos != ' ') pos++;
                }
                pos++;

                DoCpy(value, GetRegister(*pos));
            }
            else if (strncmp(instruction.c_str(), "inc ", 4) == 0)
            {
                DoInc(GetRegister(*(pos + 4)));
            }
            else if (strncmp(instruction.c_str(), "dec ", 4) == 0)
            {
                DoDec(GetRegister(*(pos + 4)));
            }
            else if (strncmp(instruction.c_str(), "jnz ", 4) == 0)
            {
                int value = 0;
                pos += 4;
                if (*pos >= 'a' && *pos <= 'd')
                {
                    value = GetRegister(*pos++);
                }
                else
                {
                    value = atoi(&*pos);
                    while (*pos != ' ') pos++;
                }
                pos++;

                int offset = atoi(&*pos);

                DoJnz(value, offset);
            }
        }
    }

    int GetA() { return a; }

private:
    int &GetRegister(char name) {
        switch (name)
        {
        case 'a': return a;
        case 'b': return b;
        case 'c': return c;
        case 'd': return d;
        default: assert(false); return a;
        }
    }
    void DoCpy(int value, int &r) { r = value; pc++; }
    void DoInc(int &r) { r++; pc++; }
    void DoDec(int &r) { r--; pc++; }
    void DoJnz(int value, int offset) { pc += value ? offset : 1; }

    int a, b, c, d;
    unsigned pc = 0;
};

void ReadProgram(const std::string &filename, Program &program)
{
    std::ifstream f(filename);
    for (std::string line; std::getline(f, line);)
        program.push_back(line);
}

int _tmain(int argc, _TCHAR *argv[])
{
    Program program;
    ReadProgram("Input.txt", program);

    Computer computer1;
    computer1.ExecuteProgram(program);
    auto partOne = computer1.GetA();
    std::cout << "Part One: " << partOne << std::endl;

    Computer computer2(1);
    computer2.ExecuteProgram(program);
    auto partTwo = computer2.GetA();
    std::cout << "Part Two: " << partTwo << std::endl;

    assert(partOne == 318077); 
    assert(partTwo == 9227731);
    return 0;
}
