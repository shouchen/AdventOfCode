#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cassert>

class Computer;

using Program = std::vector<std::string>;

class Computer
{
public:
    Computer() { InitializeRegisters(); }

    void InitializeRegisters(long long a = 0, long long b = 0, long long c = 0, long long d = 0)
    {
        GetRegister('a') = a;
        GetRegister('b') = b;
        GetRegister('c') = c;
        GetRegister('d') = d;
    }

    inline long long &GetRegister(char name)
    {
        assert(name >= 'a' && name <= 'd');
        return registers[name - 'a'];
    }

    void ExecuteProgram(Program &program);
    void ExecuteInstruction(std::string instruction);

private:
    inline static bool IsRegister(const std::string x)
    {
        return x.length() == 1 && x[0] >= 'a' && x[0] <= 'd';
    }

    void Optimize();

    Program originalProgram, program;
    long long registers[4];
    unsigned pc = 0;
};

void Computer::ExecuteProgram(Program &program)
{
    this->originalProgram.clear();
    this->program.clear();

    for (auto i : program)
    {
        this->originalProgram.push_back(i);
        this->program.push_back(i);
    }

    Optimize();
    for (pc = 0U; pc < this->program.size(); )
        ExecuteInstruction(this->program[pc]);
}

void Computer::Optimize()
{
    // Remove optimization and re-optimize
    program.clear();
    for (auto i : originalProgram)
        this->program.push_back(i);

    // Look for multiplies of the form a += c * d
    for (auto i = 0U; i < program.size() - 4; i++)
    {
        if (program[i] == "inc a")
        {
            if ((program[i + 1] == "dec c" && program[i + 2] == "jnz c -2" && program[i + 3] == "dec d" && program[i + 4] == "jnz d -5") ||
                (program[i + 1] == "dec d" && program[i + 2] == "jnz d -2" && program[i + 3] == "dec c" && program[i + 4] == "jnz c -5"))
            {
                program[i] = "mpy a";
                program[i + 1] = "cpy 0 c";
                program[i + 2] = "cpy 0 d";
                program[i + 3] = "cpy 0 c";
                program[i + 4] = "cpy 0 d";
            }
        }
    }
}

void Computer::ExecuteInstruction(std::string instruction)
{
    auto find = instruction.find(' ', 4);

    auto opcode = instruction.substr(0, 3);
    std::string op1, op2;
    if (find == std::string::npos)
    {
        op1 = instruction.substr(4);
    }
    else
    {
        op1 = instruction.substr(4, find - 4);
        op2 = instruction.substr(find + 1);
    }

    if (opcode == "cpy") // src, dest   (reg/value, reg)
    {
        if (IsRegister(op2))
        {
            long long value = IsRegister(op1) ? GetRegister(op1[0]) : atoi(op1.c_str());
            GetRegister(op2[0]) = value;
        }
        pc++;
    }
    else if (opcode == "inc") // reg
    {
        if (IsRegister(op1))
            GetRegister(op1[0])++;

        pc++;
    }
    else if (opcode == "dec") // reg
    {
        if (IsRegister(op1))
            GetRegister(op1[0])--;

        pc++;
    }
    else if (opcode == "jnz") // if op1 is not zero, jump to offset of op2
    {
        long long value = IsRegister(op1) ? GetRegister(op1[0]) : atoi(op1.c_str());
        long long offset = IsRegister(op2) ? GetRegister(op2[0]) : atoi(op2.c_str());
        auto newpc = pc + (value ? offset : 1);
        assert(newpc != pc); // catch infini-loop
        if (newpc >= 0 && newpc < program.size())
            pc = static_cast<unsigned>(newpc);
    }
    else if (opcode == "tgl") // offset and self-modify code
    {
        long long offset = IsRegister(op1) ? GetRegister(op1[0]) : atoi(op1.c_str());

        if (pc + offset >= 0 && pc + offset < program.size())
        {
            auto instruction = program[pc + static_cast<int>(offset)];
            auto opcode = instruction.substr(0, 3);

            if (opcode == "cpy") instruction = "jnz" + instruction.substr(3);
            else if (opcode == "jnz") instruction = "cpy" + instruction.substr(3);
            else if (opcode == "inc") instruction = "dec" + instruction.substr(3);
            else if (opcode == "dec") instruction = "inc" + instruction.substr(3);
            else if (opcode == "tgl") instruction = "inc" + instruction.substr(3);
            else assert(false);

            program[pc + static_cast<int>(offset)] = instruction;
            originalProgram[pc + static_cast<int>(offset)] = instruction;

            Optimize();
        }
        pc++;
    }
    else if (opcode == "add") // a += d
    {
        long long value = IsRegister(op1) ? GetRegister(op1[0]) : atoi(op1.c_str());
        GetRegister(op2[0]) += value;
        pc++;
    }
    else if (opcode == "mpy") // a += d * c
    {
        GetRegister('a') += GetRegister('d') * GetRegister('c');
        pc++;
    }
    else
        assert(false);
}

int _tmain(int argc, _TCHAR *argv[])
{
    double startTime = clock();

    Program program;

    std::ifstream f("input.txt");
    std::string line;
    while (std::getline(f, line))
        program.push_back(line);

    Computer computer;
    computer.InitializeRegisters(7, 0, 0, 0);
    computer.ExecuteProgram(program);

    auto partOne = computer.GetRegister('a');
    std::cout << "Part One: " << partOne << std::endl;

    computer.InitializeRegisters(12, 0, 0, 0);
    computer.ExecuteProgram(program);

    auto partTwo = computer.GetRegister('a');
    std::cout << "Part Two: " << partTwo << std::endl;

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;

    assert(partOne == 13468);
    assert(partTwo == 479010028);
    return 0;
}
