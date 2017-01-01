#include "stdafx.h"
#include <memory>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

class Computer;
class Instruction;

using Program = std::vector<std::unique_ptr<Instruction>>;

class Instruction
{
public:
    virtual void Execute(Computer &computer) = 0;
    virtual ~Instruction() {}
};

class Computer
{
public:
    Computer() { InitializeRegisters(); }

    void InitializeRegisters(int a = 0, int b = 0, int c = 0, int d = 0)
    {
        GetRegister('a') = a;
        GetRegister('b') = b;
        GetRegister('c') = c;
        GetRegister('d') = d;
    }

    inline int &GetRegister(char name)
    {
        assert(name >= 'a' && name <= 'd');
        return registers[name - 'a'];
    }

    inline void DoCpy(char from, char to) { DoCpy(GetRegister(from), to); }
    inline void DoCpy(int value, char to) { GetRegister(to) = value; pc++; }
    inline void DoInc(char r) { GetRegister(r)++; pc++; }
    inline void DoDec(char r) { GetRegister(r)--; pc++; }
    inline void DoJnz(char reg, int offset) { DoJnz(GetRegister(reg), offset); }
    inline void DoJnz(int value, int offset) { pc += value ? offset : 1; }

    void ExecuteProgram(const Program &Program)
    {
        for (pc = 0; pc < Program.size(); )
            Program[pc]->Execute(*this);
    }

private:
    int registers[4];
    unsigned pc = 0;
};

class CpyRegisterInstruction : public Instruction
{
public:
    CpyRegisterInstruction(char from, char to) : from(from), to(to) {}
    void Execute(Computer &computer) override { computer.DoCpy(from, to); }
private:
    char from, to;
};

class CpyValueInstruction : public Instruction
{
public:
    CpyValueInstruction(int value, char to) : value(value), to(to) {}
    void Execute(Computer &computer) override { computer.DoCpy(value, to); }
private:
    int value;
    char to;
};

class IncInstruction : public Instruction
{
public:
    IncInstruction(char reg) : reg(reg) {}
    void Execute(Computer &computer) override { computer.DoInc(reg); }
private:
    char reg;
};

class DecInstruction : public Instruction
{
public:
    DecInstruction(char reg) : reg(reg) {}
    void Execute(Computer &computer) override { computer.DoDec(reg); }
private:
    char reg;
};

class JnzRegisterInstruction : public Instruction
{
public:
    JnzRegisterInstruction(char reg, int offset) : reg(reg), offset(offset) {}
    void Execute(Computer &computer) override { computer.DoJnz(reg, offset); }
private:
    char reg;
    int offset;
};

class JnzValueInstruction : public Instruction
{
public:
    JnzValueInstruction(int value, int offset) : value(value), offset(offset) {}
    void Execute(Computer &computer) override { computer.DoJnz(value, offset); }
private:
    int value, offset;
};

class Assembler
{
public:
    static void AssembleProgram(const std::string &filename, Program &program)
    {
        std::ifstream f(filename);
        std::string opcode, op1, op2;

        while (f >> opcode)
        {
            if (opcode == "cpy")
            {
                f >> op1 >> op2;
                IsRegister(op1)
                    ? program.push_back(std::make_unique<CpyRegisterInstruction>(op1[0], op2[0]))
                    : program.push_back(std::make_unique<CpyValueInstruction>(atoi(op1.c_str()), op2[0]));
            }
            else if (opcode == "inc")
            {
                f >> op1;
                program.push_back(std::make_unique<IncInstruction>(op1[0]));
            }
            else if (opcode == "dec")
            {
                f >> op1;
                program.push_back(std::make_unique<DecInstruction>(op1[0]));
            }
            else if (opcode == "jnz")
            {
                f >> op1 >> op2;
                IsRegister(op1)
                    ? program.push_back(std::make_unique<JnzRegisterInstruction>(op1[0], atoi(op2.c_str())))
                    : program.push_back(std::make_unique<JnzValueInstruction>(atoi(op1.c_str()), atoi(op2.c_str())));
            }
            else
                assert(false);
        }
    }

private:
    inline static bool IsRegister(const std::string x)
    {
        return x.length() == 1 && x[0] >= 'a' && x[0] <= 'd';
    }
};

int _tmain(int argc, _TCHAR *argv[])
{
    Program program;
    Assembler::AssembleProgram("input.txt", program);

    Computer computer;
    computer.ExecuteProgram(program);

    auto partOne = computer.GetRegister('a');
    std::cout << "Part One: " << partOne << std::endl;

    computer.InitializeRegisters(0, 0, 1, 0);
    computer.ExecuteProgram(program);

    auto partTwo = computer.GetRegister('a');
    std::cout << "Part Two: " << partTwo << std::endl;

    assert(partOne == 318077);
    assert(partTwo == 9227731);
    return 0;
}
