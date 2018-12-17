#include "stdafx.h"
#include <iostream>
#include <fstream>
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

std::vector<Instruction> program;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string opcode, a, b, c;

    while (file >> opcode >> a >> b >> c)
        program.push_back(Instruction{ opcode, a, b, c });
}

int reg[4];

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
    else if (opcode == "grrr")
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

bool TestExample(int before[4], const std::string &opcode, int a, int b, int c, int after[4])
{
    for (int i = 0; i < 4; i++)
        reg[i] = before[i];

    do_instruction(opcode, a, b, c);

    for (int i = 0; i < 4; i++)
        if (reg[i] != after[i])
            return false;

    return true;
}

int main()
{
    std::string opcodes[] =
    {
        "addr", "addi", "mulr", "muli", "banr", "bani", "borr" ,"bori",
        "setr", "seti", "gtir", "gtri", "grrr", "eqir", "eqri", "eqrr"
    };

    std::ifstream file("input.txt");
    std::string before_colon, after_colon;
    char open_brace, close_brace, comma;
    int opcode, a, b, c;
    int before[4], after[4];

    unsigned count_of_three_or_more = 0;

    while (file >> before_colon >> open_brace >> before[0] >> comma >> before[1] >> comma >> before[2] >> comma >> before[3] >> close_brace
        >> opcode >> a >> b >> c
        >> after_colon >> open_brace >> after[0] >> comma >> after[1] >> comma >> after[2] >> comma >> after[3] >> close_brace)
    {
        int unsigned num_successful = 0;
        int successful_index = -1;
        for (int i = 0; i < 16; i++)
        {
            if (TestExample(before, opcodes[i], a, b, c, after))
            {
                num_successful++;
                successful_index = i;
            }
        }

        if (num_successful >= 3)
            count_of_three_or_more++;
    }

    std::cout << count_of_three_or_more << std::endl;

    // PART2
    std::map<int, std::string> opcode_map;
    std::set<std::string> have_opcode;
    while (opcode_map.size() < 16)
    {
        std::ifstream file2("input.txt");

        while (file2 >> before_colon >> open_brace >> before[0] >> comma >> before[1] >> comma >> before[2] >> comma >> before[3] >> close_brace
            >> opcode >> a >> b >> c
            >> after_colon >> open_brace >> after[0] >> comma >> after[1] >> comma >> after[2] >> comma >> after[3] >> close_brace)
        {
            // Already know what this opcode is
            if (opcode_map.find(opcode) != opcode_map.end())
                continue;

            int unsigned num_successful = 0;
            int successful_index = -1;
            for (int i = 0; i < 16; i++)
            {
                // Don't try to match this opcode since we already have it.
                if (have_opcode.find(opcodes[i]) != have_opcode.end())
                    continue;

                if (TestExample(before, opcodes[i], a, b, c, after))
                {
                    num_successful++;
                    successful_index = i;
                }
            }

            if (num_successful == 1)
            {
                opcode_map[opcode] = opcodes[successful_index];
                have_opcode.insert(opcodes[successful_index]);
            }
        }
    }

    // EXECUTE PROGRAM
    std::ifstream file3("input2.txt");

    reg[0] = reg[1] = reg[2] = reg[3] = 0;

    while (file3 >> opcode >> a >> b >> c)
    {
        do_instruction(opcode_map[opcode], a, b, c);
    }



    return 0;
}
// 560 right!
// 622 right for part2!