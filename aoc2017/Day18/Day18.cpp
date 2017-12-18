#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <queue>

std::vector<std::string> program;

long long sound = -1;
long long part1 = -1;

unsigned part2;

struct Process
{
    Process(unsigned instance) : instance(instance) {}
    void init();
    bool do_instruction(bool for_part2); // part1 true = done; part2 true = blocked

    unsigned instance;
    std::map<std::string, long long> registers;
    unsigned curr;
    std::queue<long long> myq, otherq;
};

std::queue<long long> q0, q1;
Process p0(0), p1(1);

void Process::init()
{
    registers["a"] = 0;
    registers["b"] = 0;
    registers["c"] = 0;
    registers["d"] = 0;
    registers["e"] = 0;
    registers["f"] = 0;
    registers["g"] = 0;
    registers["h"] = 0;
    registers["i"] = 0;
    registers["j"] = 0;
    registers["k"] = 0;
    registers["l"] = 0;
    registers["m"] = 0;
    registers["n"] = 0;
    registers["o"] = 0;
    registers["p"] = instance;
    registers["q"] = 0;
    registers["r"] = 0;
    registers["s"] = 0;
    registers["t"] = 0;
    registers["u"] = 0;
    registers["v"] = 0;
    registers["w"] = 0;
    registers["x"] = 0;
    registers["y"] = 0;
    registers["z"] = 0;
    curr = 0;
}

bool Process::do_instruction(bool for_part2)
{
    std::string instruction, op1, op2;
    long long val1, val2;
    std::istringstream ss(program[curr]);
    ss >> instruction;

    if (instruction == "snd")
    {
        ss >> op1;
        val1 = isalpha(op1[0]) ? registers[op1] : _atoi64(op1.c_str());

        if (for_part2)
        {
            if (instance == 0)
            {
                q1.push(val1);
            }
            else
            {
                q0.push(val1);
                part2++;
            }
        }
        else
        {
            sound = val1;
        }
        curr++;
    }
    else if (instruction == "set")
    {
        ss >> op1 >> op2;
        val2 = isalpha(op2[0]) ? registers[op2] : _atoi64(op2.c_str());
        registers[op1] = val2;
        curr++;
    }
    else if (instruction == "add")
    {
        ss >> op1 >> op2;
        val2 = isalpha(op2[0]) ? registers[op2] : _atoi64(op2.c_str());
        registers[op1] += val2;
        curr++;
    }
    else if (instruction == "mul")
    {
        ss >> op1 >> op2;
        val2 = isalpha(op2[0]) ? registers[op2] : _atoi64(op2.c_str());
        registers[op1] *= val2;
        curr++;
    }
    else if (instruction == "mod")
    {
        ss >> op1 >> op2;
        val2 = isalpha(op2[0]) ? registers[op2] : _atoi64(op2.c_str());
        registers[op1] %= val2;
        curr++;
    }
    else if (instruction == "rcv")
    {
        ss >> op1;
        val1 = isalpha(op1[0]) ? registers[op1] : _atoi64(op1.c_str());

        if (for_part2)
        {
            if (instance == 0)
            {
                if (!q0.empty())
                {
                    registers[op1] = q0.front();
                    q0.pop();
                    curr++;
                }
                else return true;
            }
            else
            {
                if (!q1.empty())
                {
                    registers[op1] = q1.front();
                    q1.pop();
                    curr++;
                }
                else return true;
            }
        }
        else
        {
            if (val1 != 0)
            {
                part1 = sound;
                return true;
            }

        }
    }
    else if (instruction == "jgz")
    {
        ss >> op1 >> op2;
        val1 = isalpha(op1[0]) ? registers[op1] : _atoi64(op1.c_str());
        val2 = isalpha(op2[0]) ? registers[op2] : _atoi64(op2.c_str());
        if (val1 > 0)
            curr += val2;
        else
            curr++;
    }

    return false;
}


int main()
{
    p0.init();
    p1.init();

    std::ifstream f("input.txt");
    std::string instruction, op1, op2;

    while (getline(f, instruction))
        program.push_back(instruction);

    while (p0.curr < program.size() && p1.curr < program.size())
    {
        bool blocked0, blocked1;

        if (p0.curr < program.size())
            blocked0 = p0.do_instruction(true);

        if (p1.curr < program.size())
            blocked1 = p1.do_instruction(true);

        if (blocked0 && blocked1)
            break;
    }

    std::cout << "Part 2:" << part2 << std::endl;

    // PART ONE
    p0.init();

    while (p0.curr < program.size())
    {
        if (p0.do_instruction(false) == true)
            break;
    }

    std::cout << "Part 1: " << part1 << std::endl;

    assert(part1 == 9423);
    assert(part2 == 7620);
    return 0;
}
