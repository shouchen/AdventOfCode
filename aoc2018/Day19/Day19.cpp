#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <cassert>

using namespace std::string_literals;
using Registers = std::array<int, 6>;

struct Instruction
{
    std::string mnemonic;
    int a, b, c;
};

std::vector<Instruction> program;
Registers reg;

void do_instruction(const Instruction &instruction)
{
    auto mnemonic = instruction.mnemonic;
    auto a = instruction.a, b = instruction.b, c = instruction.c;

    if      (mnemonic == "addr") reg[c] = reg[a] + reg[b];
    else if (mnemonic == "addi") reg[c] = reg[a] + b;
    else if (mnemonic == "mulr") reg[c] = reg[a] * reg[b];
    else if (mnemonic == "muli") reg[c] = reg[a] * b;
    else if (mnemonic == "banr") reg[c] = reg[a] & reg[b];
    else if (mnemonic == "bani") reg[c] = reg[a] & b;
    else if (mnemonic == "borr") reg[c] = reg[a] | reg[b];
    else if (mnemonic == "bori") reg[c] = reg[a] | b;
    else if (mnemonic == "setr") reg[c] = reg[a];
    else if (mnemonic == "seti") reg[c] = a;
    else if (mnemonic == "gtir") reg[c] = (a > reg[b]) ? 1 : 0;
    else if (mnemonic == "gtri") reg[c] = (reg[a] > b) ? 1 : 0;
    else if (mnemonic == "gtrr") reg[c] = (reg[a] > reg[b]) ? 1 : 0;
    else if (mnemonic == "eqir") reg[c] = (a == reg[b]) ? 1 : 0;
    else if (mnemonic == "eqri") reg[c] = (reg[a] == b) ? 1 : 0;
    else if (mnemonic == "eqrr") reg[c] = (reg[a] == reg[b]) ? 1 : 0;
    else assert(false);
}

auto run_program(int initial_r0, int ip_reg)
{
    auto &ip = reg[ip_reg];

    std::fill(reg.begin(), reg.end(), 0);
    reg[0] = initial_r0;

    for (;;)
    {
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
            ip = 16;
        }
        else
        {
            do_instruction(program[ip]);
        }

        if (++ip >= program.size())
            break;
    }

    return reg[0];
}

int main()
{
    std::ifstream file("input.txt");

    auto pound_ip = "#ip"s, mnemonic = ""s;
    auto ip_reg = 0, a = 0, b = 0, c = 0;

    file >> pound_ip >> ip_reg;

    while (file >> mnemonic >> a >> b >> c)
        program.push_back(Instruction{ mnemonic, a, b, c });

    auto part1 = run_program(0, ip_reg);
    auto part2 = run_program(1, ip_reg);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 1764);
    assert(part2 == 18992484);

    return 0;
}
