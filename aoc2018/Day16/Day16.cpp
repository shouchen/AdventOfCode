#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <array>
#include <map>
#include <set>
#include <string>
#include <cassert>

using namespace std::string_literals;

enum Mnemonic
{
    ADDR, ADDI, MULR, MULI, BANR, BANI, BORR, BORI,
    SETR, SETI, GTIR, GTRI, GRRR, EQIR, EQRI, EQRR,
    NumMnemonics
};

struct Instruction
{
    Mnemonic mnemonic;
    int a, b, c;
};

using Registers = std::array<int, 4>;

Registers reg;
std::map<int, Mnemonic> opcode_to_mnemonic;
std::set<Mnemonic> know_opcode;

void do_instruction(Mnemonic mnemonic, int a, int b, int c)
{
    switch (mnemonic)
    {
    case ADDR:  reg[c] = reg[a] + reg[b];            break;
    case ADDI:  reg[c] = reg[a] + b;                 break;
    case MULR:  reg[c] = reg[a] * reg[b];            break;
    case MULI:  reg[c] = reg[a] * b;                 break;
    case BANR:  reg[c] = reg[a] & reg[b];            break;
    case BANI:  reg[c] = reg[a] & b;                 break;
    case BORR:  reg[c] = reg[a] | reg[b];            break;
    case BORI:  reg[c] = reg[a] | b;                 break;
    case SETR:  reg[c] = reg[a];                     break;
    case SETI:  reg[c] = a;                          break;
    case GTIR:  reg[c] = (a > reg[b]) ? 1 : 0;       break;
    case GTRI:  reg[c] = (reg[a] > b) ? 1 : 0;       break;
    case GRRR:  reg[c] = (reg[a] > reg[b]) ? 1 : 0;  break;
    case EQIR:  reg[c] = (a == reg[b]) ? 1 : 0;      break;
    case EQRI:  reg[c] = (reg[a] == b) ? 1 : 0;      break;
    case EQRR:  reg[c] = (reg[a] == reg[b]) ? 1 : 0; break;
    default: assert(false);
    }
}

void execute_program(const std::string &filename)
{
    std::fill(reg.begin(), reg.end(), 0);

    std::ifstream file(filename);
    auto opcode = 0, a = 0, b = 0, c = 0;

    while (file >> opcode >> a >> b >> c)
        do_instruction(opcode_to_mnemonic[opcode], a, b, c);
}

auto do_part(bool for_part2)
{
    while (know_opcode.size() < NumMnemonics)
    {
        std::ifstream file("input.txt");
        auto before_colon = "Before:"s, after_colon = "After:"s;
        auto open_brace = '{', close_brace = '}', comma = ',';
        auto opcode = 0, a = 0, b = 0, c = 0;
        Registers before, after;
        auto num_with_three_or_more_candidates = 0;

        while (file >> before_colon >> open_brace >> before[0] >> comma >> before[1] >> comma >> before[2] >> comma >> before[3]
            >> close_brace >> opcode >> a >> b >> c >> after_colon >> open_brace >> after[0] >> comma >> after[1] >> comma
            >> after[2] >> comma >> after[3] >> close_brace)
        {
            if (opcode_to_mnemonic.find(opcode) != opcode_to_mnemonic.end())
                continue;

            auto num_mnemonic_candidates = 0U;
            auto inferred_mnemonic = NumMnemonics;

            for (auto i = 0; i < NumMnemonics; i++)
            {
                auto mnemonic = static_cast<Mnemonic>(i);

                // Don't try to match this mnemonic since we already have it.
                if (know_opcode.find(mnemonic) != know_opcode.end())
                    continue;

                reg = before;
                do_instruction(mnemonic, a, b, c);
                if (reg == after)
                    inferred_mnemonic = (++num_mnemonic_candidates == 1) ? mnemonic : NumMnemonics;
            }

            if (num_mnemonic_candidates >= 3)
            {
                num_with_three_or_more_candidates++;
            }
            else if (for_part2 && inferred_mnemonic != NumMnemonics)
            {
                opcode_to_mnemonic[opcode] = inferred_mnemonic;
                know_opcode.insert(static_cast<Mnemonic>(inferred_mnemonic));
            }
        }

        if (!for_part2)
            return num_with_three_or_more_candidates;
    }

    execute_program("input2.txt");
    return reg[0];
}

int main()
{
    auto part1 = do_part(false);
    auto part2 = do_part(true);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 560);
    assert(part2 == 622);
    return 0;
}
