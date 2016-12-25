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

    bool ExecuteProgram(Program &program);

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

    bool ExecuteInstruction(std::string instruction);
    void Optimize();

    Program originalProgram, program;
    unsigned pc = 0;

    long long lastOutput = 0;
    unsigned outputLength = 0;
    static const unsigned outputCap = 50;
};

bool Computer::ExecuteProgram(Program &program)
{
    lastOutput = 1;
    outputLength = 0;

    this->originalProgram.clear();

    for (auto i : program)
        this->originalProgram.push_back(i);

    Optimize();

    for (pc = 0U; pc < this->program.size() && outputLength < outputCap; )
        if (!ExecuteInstruction(this->program[pc]))
            return false;

    return true;
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
        else if (program[i] == "inc d")
        {
            if ((program[i + 1] == "dec b" && program[i + 2] == "jnz b -2" && program[i + 3] == "dec c" && program[i + 4] == "jnz c -5"))
            {
                program[i] = "mpy b c d";
                program[i + 1] = "cpy 0 b";
                program[i + 2] = "cpy 0 c";
                program[i + 3] = "cpy 0 b";
                program[i + 4] = "cpy 0 c";
            }
        }
    }
}

bool Computer::ExecuteInstruction(std::string instruction)
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
    else if (opcode == "out")
    {
        long long value = IsRegister(op1) ? GetRegister(op1[0]) : atoi(op1.c_str());
        std::cout << value;

        // If right output pattern not seen, abort
        if ((lastOutput == 1 && value != 0) || (lastOutput == 0 && value != 1))
        {
            std::cout << "..." << std::endl;
            return false;
        }

        // If first 50 characters of output are right, stop the infinite loop
        if (++outputLength == outputCap)
            std::cout << "..." << std::endl;

        lastOutput = value;
        pc++;
    }
    else
        assert(false);

    return true;
}

// For this problem, each successive starting "a" value will be tried, and if the output
// isn't the target one, the execution of that attempt will be terminated. Once the
// correct value for a is found, the output will spew 01010101... Since, for any initial
// value of a, the program never terminates, we'll cap the output at the first 50 bits.
//
// Reverse-engineering the input program, it's doing this:
// 1. Compute 643 * 4 and add this to a's initial value.
// 2. Stream the binary representation of step 1 starting from the LSB until the most-
//    significant 1 bit has been streamed.
// 3. Repeat step 2 infinitely.
//
// When register a (program input) is 0, we start with 2572 (from the multiplication).
// In binary this is 101000001100, which streams repeatedly (in the reverse order). But
// since we need a streaming pattern of alternating ones and zeros, the next such
// binary number would be 101010101010, which is 2730. So the lowest qualifying starting
// value for a has to be 158. And this also tells you that the repeating output pattern
// would indeed go on ad infinitum.

int _tmain(int argc, _TCHAR *argv[])
{
    double startTime = clock();

    Program program;

    std::ifstream f("input.txt");
    std::string line;
    while (std::getline(f, line))
        program.push_back(line);

    auto part1 = 0LL;

    Computer computer;
    for (auto a = 1LL; a; a++)
    {
        std::cout << "a=" << a << ": ";

        computer.InitializeRegisters(a, 0, 0, 0);
        if (computer.ExecuteProgram(program))
        {
            part1 = a;
            break;
        }
    }

    std::cout << "Part One: " << part1 << std::endl;

    std::cout << std::endl << "It took " << (clock() - startTime) / CLOCKS_PER_SEC << " sec." << std::endl;

    assert(part1 == 158);
    return 0;
}
