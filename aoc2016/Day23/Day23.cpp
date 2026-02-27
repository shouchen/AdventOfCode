#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <cassert>

using Program = std::vector<std::string>;

class Computer
{
public:
    Computer() { InitializeRegisters(); }

    void InitializeRegisters(long long a = 0, long long b = 0, long long c = 0, long long d = 0)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
    }

    void ExecuteProgram(Program &program);

    long long a, b, c, d;

private:
    inline static bool IsRegister(const std::string x)
    {
        return x.length() == 1 && x[0] >= 'a' && x[0] <= 'd';
    }

    inline long long &GetRegister(char name)
    {
        switch (name)
        {
        case 'a': return a;
        case 'b': return b;
        case 'c': return c;
        case 'd': return d;
        default:
            assert(false);
            return a;
        }
    }

    void ExecuteInstruction(std::string instruction);
    void Optimize();

    Program originalProgram, program;
    unsigned pc = 0;
};

void Computer::ExecuteProgram(Program &program)
{
    this->originalProgram.clear();

    for (auto i : program)
        this->originalProgram.push_back(i);

    Optimize();

    for (pc = 0U; pc < this->program.size(); )
        ExecuteInstruction(this->program[pc]);
}

void Computer::Optimize()
{
    // Code self-modified; remove any optimizations and re-optimize
    program.clear();
    for (auto i : originalProgram)
        this->program.push_back(i);

    // Replace slow multiplies of the form a += c * d
    for (auto i = 0U; i < program.size() - 4; i++)
    {
        if (program[i] == "inc a")
        {
            if ((program[i + 1] == "dec c" && program[i + 2] == "jnz c -2" && program[i + 3] == "dec d" && program[i + 4] == "jnz d -5") ||
                (program[i + 1] == "dec d" && program[i + 2] == "jnz d -2" && program[i + 3] == "dec c" && program[i + 4] == "jnz c -5"))
            {
                program[i] = "mpy c d a";
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
    std::stringstream ss(instruction);
    std::string opcode, op1, op2, op3;
    ss >> opcode >> op1 >> op2 >> op3;

    if (opcode == "cpy")
    {
        if (IsRegister(op2))
        {
            long long value = IsRegister(op1) ? GetRegister(op1[0]) : atoi(op1.c_str());
            GetRegister(op2[0]) = value;
        }
        pc++;
    }
    else if (opcode == "inc")
    {
        if (IsRegister(op1))
            GetRegister(op1[0])++;

        pc++;
    }
    else if (opcode == "dec")
    {
        if (IsRegister(op1))
            GetRegister(op1[0])--;

        pc++;
    }
    else if (opcode == "jnz")
    {
        long long value = IsRegister(op1) ? GetRegister(op1[0]) : atoi(op1.c_str());
        long long offset = IsRegister(op2) ? GetRegister(op2[0]) : atoi(op2.c_str());

        auto newpc = pc + (value ? offset : 1);
        if (newpc >= 0 && newpc < program.size())
            pc = static_cast<unsigned>(newpc);
    }
    else if (opcode == "tgl")
    {
        long long offset = IsRegister(op1) ? GetRegister(op1[0]) : atoi(op1.c_str());
        if (pc + offset >= 0 && pc + offset < program.size())
        {
            auto instruction = originalProgram[pc + static_cast<int>(offset)];
            auto opcode = instruction.substr(0, 3);

            if (opcode == "cpy") instruction.replace(0, 3, "jnz");
            else if (opcode == "jnz") instruction.replace(0, 3, "cpy");
            else if (opcode == "inc") instruction.replace(0, 3, "dec");
            else if (opcode == "dec" || opcode == "tgl") instruction.replace(0, 3, "inc");
            else assert(false);

            originalProgram[pc + static_cast<int>(offset)] = instruction;
            Optimize();
        }
        pc++;
    }
    else if (opcode == "mpy")
    {
        if (IsRegister(op1) && IsRegister(op2) && IsRegister(op3))
            GetRegister(op3[0]) += GetRegister(op1[0]) * GetRegister(op2[0]);

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

    auto partOne = computer.a;
    std::cout << "Part One: " << partOne << std::endl;

    computer.InitializeRegisters(12, 0, 0, 0);
    computer.ExecuteProgram(program);

    auto partTwo = computer.a;
    std::cout << "Part Two: " << partTwo << std::endl;

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;

    assert(partOne == 13468);
    assert(partTwo == 479010028);
    return 0;
}
