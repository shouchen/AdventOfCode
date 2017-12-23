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

    inline auto get_times_multiplied() { return times_multiplied; }

private:
    long long evaluate(const std::string &expr);

    unsigned curr, times_multiplied;
    std::vector<std::string> &program;
    std::map<char, long long> registers;
};

Process::Process(std::vector<std::string> &program) :
    curr(0), times_multiplied(0), program(program)
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
        times_multiplied++;
    }
    else if (opcode == "jnz")
    {
        if (evaluate(op1) != 0)
            curr = unsigned(curr + evaluate(op2) - 1);
    }

    return ++curr < program.size();
}

long long Process::evaluate(const std::string &expr)
{
    return isalpha(expr[0]) ? registers[expr[0]] : _atoi64(expr.c_str());
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
    return p.get_times_multiplied();
}

bool is_prime(long long b)
{
    for (long long d = 2; d < sqrt(b); d++)
        if (b % d == 0)
            return 0;

    return 1;
}

long long do_part2()
{
    long long h = 0;

    for (long long b = 107900; b <= 124900; b += 17)
    {
        //long long f = 1;

        //for (long long d = 2; d != b && f; d++)
        //{
        //    for (long long e = 2; e != b && f; e++)
        //    {
        //        if (d * e == b)
        //        {
        //            f = 0;
        //        }
        //    }
        //}
        long long f = is_prime(b);

        if (f == 0)
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
