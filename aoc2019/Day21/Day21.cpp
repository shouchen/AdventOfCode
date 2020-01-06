#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <array>
#include <cassert>

class Computer
{
public:
    Computer(const std::string &program, std::queue<long long> &input, std::queue<long long> &output)
        : input(input), output(output)
    {
        std::ifstream file(program);
        auto a = 0LL, address = 0LL;
        auto comma = ',';

        while (file >> a)
        {
            data[address++] = a;
            file >> comma;
        }
    }

    void execute_program_until_blocked()
    {
        while (execute_instruction());
    }

    bool halted = false;
    std::map<long long, long long> data;

private:
    enum Opcode { Add = 1, Multiply = 2, Input = 3, Output = 4, JumpIfTrue = 5, JumpIfFalse = 6, LessThan = 7, Equals = 8, Rebase = 9, Halt = 99 };

    static const std::array<int, 4> param_shift;

    std::queue<long long> &input, &output;
    long long pc = 0, relative_base = 0;

    Opcode get_instruction() { return Opcode(data[pc] % 100); }

    auto &get_param(unsigned n) {
        auto mode = (data[pc] / param_shift[n]) % 10;
        if (mode == 0) return data[data[pc + n]];
        if (mode == 1) return data[pc + n];
        return data[data[pc + n] + relative_base];
    }

    bool execute_instruction()
    {
        switch (get_instruction())
        {
        case Add:
            get_param(3) = get_param(1) + get_param(2);
            pc += 4;
            break;
        case Multiply:
            get_param(3) = get_param(1) * get_param(2);
            pc += 4;
            break;
        case Input:
            if (input.empty())
                return false;
            get_param(1) = input.front();
            input.pop();
            pc += 2;
            break;
        case Output:
            output.push(get_param(1));
            pc += 2;
            break;
        case JumpIfTrue:
            pc = (get_param(1) != 0) ? get_param(2) : (pc + 3);
            break;
        case JumpIfFalse:
            pc = (get_param(1) == 0) ? get_param(2) : (pc + 3);
            break;
        case LessThan:
            get_param(3) = (get_param(1) < get_param(2)) ? 1 : 0;
            pc += 4;
            break;
        case Equals:
            get_param(3) = (get_param(1) == get_param(2)) ? 1 : 0;
            pc += 4;
            break;
        case Rebase:
            relative_base += get_param(1);
            pc += 2;
            break;
        default:
            assert(false);
        case Halt:
            halted = true;
            return false;
        }
        return true;
    }
};

const std::array<int, 4> Computer::param_shift{ 0, 100, 1000, 10000 };

// Part 1: A jump will land on D, so it must be safe to jump there. Logic, then,
// is if any of {A, B, C} are unsafe but D is safe, then jump. Boolean
// expression is (!A + !B + !C) * D.
std::vector<std::string> program1{
    "NOT A J",
    "NOT B T",
    "OR T J",
    "NOT C T",
    "OR T J",
    "AND D J",
    "WALK"
};

// Part 2: Same as above, but must look at the next jump as well as the current
// one. A jump now, would land on D, and we would immediately either have to
// jump again (landing on H) or not jump (landing on E). So a second condition
// is that at least one of those second landing places has to be safe before
// making the first jump. Adding this condition we get:
//
//     (!A + !B + !C) * D * (E + H)
//
// Due to our limited instruction set, we need to rewrite the last term as this:
//
//     (!A + !B + !C) * D * (E * !H + H)

std::vector<std::string> program2{
    "NOT A J",
    "NOT B T",
    "OR T J",
    "NOT C T",
    "OR T J",
    "AND D J", // up to here, same as program 1
    "NOT H T",
    "AND E T",
    "OR H T",
    "AND T J",
    "RUN"
};

auto run_program(std::vector<std::string> &program)
{
    std::queue<long long> input, output;
    Computer c("input.txt", input, output);

    c.execute_program_until_blocked();

    while (!output.empty())
    {
        std::cout << char(output.front());
        output.pop();
    }

    for (auto &line : program)
    {
        for (auto c : line)
        {
            std::cout << c;
            input.push(c);
        }
        std::cout << std::endl;
        input.push('\n');
    }

    c.execute_program_until_blocked();

    while (!output.empty())
    {
        auto val = output.front();
        if (val > 255)
            return val;

        std::cout << char(output.front());
        output.pop();
    }

    return -1LL;
}

int main()
{
    auto part1 = run_program(program1);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = run_program(program2);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 19352864);
    assert(part2 == 1142488337);
    return 0;
}
