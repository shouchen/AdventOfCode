#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <array>
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
    std::string a, b, c;
};

enum InstructionName
{
    ADDR, ADDI, MULR, MULI, BANR, BANI, BORR, BORI,
    SETR, SETI, GTIR, GTRI, GRRR, EQIR, EQRI, EQRR,
    NumInstructions
};

std::map<int, InstructionName> opcode_map;
std::vector<Instruction> program;
std::array<int, 4> reg;

void do_instruction(InstructionName instruction, int a, int b, int c)
{
    switch (instruction)
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
    std::ifstream file3(filename);
    reg[0] = reg[1] = reg[2] = reg[3] = 0;
    auto opcode = 0, a = 0, b = 0, c = 0;

    while (file3 >> opcode >> a >> b >> c)
        do_instruction(opcode_map[opcode], a, b, c);
}

auto test_example(std::array<int, 4> &before, InstructionName instruction, int a, int b, int c, std::array<int, 4> &after)
{
    for (auto i = 0; i < 4; i++)
        reg[i] = before[i];

    do_instruction(instruction, a, b, c);

    for (auto i = 0; i < 4; i++)
        if (reg[i] != after[i])
            return false;

    return true;
}

unsigned do_part1()
{
    std::ifstream file("input.txt");
    std::string before_colon, after_colon;
    auto open_brace = '{', close_brace = '}', comma = ',';
    auto opcode = 0, a = 0, b = 0, c = 0;
    std::array<int, 4> before, after;

    auto count_of_three_or_more = 0U;

    while (file >> before_colon >> open_brace >> before[0] >> comma >> before[1] >> comma >> before[2] >> comma >> before[3]
        >> close_brace >> opcode >> a >> b >> c >> after_colon >> open_brace >> after[0] >> comma >> after[1] >> comma
        >> after[2] >> comma >> after[3] >> close_brace)
    {
        auto num_successful = 0U;
        auto successful_index = -1;
        for (auto i = 0; i < NumInstructions; i++)
        {
            if (test_example(before, (InstructionName)i, a, b, c, after))
            {
                num_successful++;
                successful_index = i;
            }
        }

        if (num_successful >= 3)
            count_of_three_or_more++;
    }

    return count_of_three_or_more;
}

int do_part2()
{
    std::set<InstructionName> have_opcode;

    while (opcode_map.size() < 16)
    {
        std::ifstream file2("input.txt");
        std::string before_colon, after_colon;
        auto open_brace = '{', close_brace = '}', comma = ',';
        auto opcode = 0, a = 0, b = 0, c = 0;
        std::array<int, 4> before, after;

        while (file2 >> before_colon >> open_brace >> before[0] >> comma >> before[1] >> comma >> before[2] >> comma >> before[3]
            >> close_brace >> opcode >> a >> b >> c >> after_colon >> open_brace >> after[0] >> comma >> after[1] >> comma
            >> after[2] >> comma >> after[3] >> close_brace)
        {
            // Already know what this opcode is
            if (opcode_map.find(opcode) != opcode_map.end())
                continue;

            auto num_successful = 0U;
            auto successful_instruction_name = NumInstructions;
            auto instruction_name = NumInstructions;
            for (auto i = 0; i < NumInstructions; i++)
            {
                auto instruction_name = static_cast<InstructionName>(i);
                // Don't try to match this opcode since we already have it.
                if (have_opcode.find(instruction_name) != have_opcode.end())
                    continue;

                if (test_example(before, instruction_name, a, b, c, after))
                {
                    num_successful++;
                    successful_instruction_name = instruction_name;
                }
            }

            if (num_successful == 1)
            {
                opcode_map[opcode] = successful_instruction_name;
                have_opcode.insert(static_cast<InstructionName>(successful_instruction_name));
            }
        }
    }

    execute_program("input2.txt");
    return reg[0];
}

int main()
{
    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 560);
    assert(part2 == 622);
    return 0;
}
