#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
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

int run_program(int initial_r0, int ip_reg)
{
    int ip = 0;

    reg[0] = initial_r0;
    reg[1] = reg[2] = reg[3] = reg[4] = reg[5] = 0;

    for (;;)
    {
        reg[ip_reg] = ip;

        // Intercept and replace slow part (manually disassembled and manually optimized).
        // This replaces a super-inefficient implementation of summing up all the factors
        // in a very large number.
        if (ip == 1)
        {
            // Sum all the factors of the number in R4, and store sum in R0.
            for (reg[3] = 1; reg[3] <= reg[4]; reg[3]++)
                if (reg[4] % reg[3] == 0)
                    reg[0] += reg[3];

            // Jump past the rest of the old, slow code.
            reg[1] = 16;
        }
        else
        {
            do_instruction(program[ip].opcode, program[ip].a, program[ip].b, program[ip].c);
        }

        ip = reg[ip_reg];
        ip++;
        if (ip >= program.size())
            break;
    }

    return reg[0];
}

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

    auto part1 = run_program(0, ip_reg);
    auto part2 = run_program(1, ip_reg);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 1764);
    assert(part2 == 18992484);

    return 0;
}
