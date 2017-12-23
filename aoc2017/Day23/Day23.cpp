#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>

class Process
{
public:
    Process(std::vector<std::string> &program);
    bool do_instruction();

    inline auto get_num_multiplies() { return num_multiplies; }

private:
    int evaluate(const std::string &expr);

    int curr;
    unsigned num_multiplies;
    std::vector<std::string> &program;
    std::map<char, int> registers;
};

Process::Process(std::vector<std::string> &program) :
    curr(0), num_multiplies(0), program(program)
{
}

bool Process::do_instruction()
{
    std::istringstream ss(program[curr]);
    std::string opcode, op1, op2;
    ss >> opcode >> op1 >> op2;

    if (opcode == "sub")
    {
        registers[op1[0]] -= evaluate(op2);
    }
    else if (opcode == "set")
    {
        registers[op1[0]] = evaluate(op2);
    }
    else if (opcode == "mul")
    {
        registers[op1[0]] *= evaluate(op2);
        num_multiplies++;
    }
    else if (opcode == "jnz")
    {
        if (evaluate(op1) != 0)
            curr = unsigned(curr + evaluate(op2) - 1);
    }

    return ++curr < program.size() && curr >= 0;
}

int Process::evaluate(const std::string &expr)
{
    return isalpha(expr[0]) ? registers[expr[0]] : atoi(expr.c_str());
}

auto read_program(const std::string &filename)
{
    std::ifstream f(filename);
    std::string instruction;
    std::vector<std::string> program;

    while (getline(f, instruction))
        program.push_back(instruction);

    return program;
}

long long do_part1(std::vector<std::string> &program)
{
    Process p(program);
    while (p.do_instruction());
    return p.get_num_multiplies();
}

// This is the (manually) disassembled code that was given as the problem input
// (with register a initialized to 1). Reading this simpler code, it's easy to
// see that the deeply-nested part is just doing a (poorly-implemented) primality
// check for b. By replacing this part of the original code with a faster check,
// the program completes in a far more reasonable time. In all, this program is
// simply making a count of the composite numbers that b takes on during the
// outer loop.
auto do_part2()
{
    auto h = 0;

    for (auto b = 107900; b <= 124900; b += 17)
    {
        auto f = true;

#if USE_ORIGINAL_CODE
        for (auto d = 2; d != b; d++)
            for (auto e = 2; e != b; e++)
                if (d * e == b)
                    f = false;
#else
        for (auto d = 2; d < sqrt(b); d++)
            if (b % d == 0)
            {
                f = false;
                break;
            }
#endif

        if (!f)
            h++;
    }

    return h;
}

int main()
{
    auto program = read_program("input.txt");
    auto part1 = do_part1(program);
    auto part2 = do_part2();

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 5929);
    assert(part2 == 907);
    return 0;
}
