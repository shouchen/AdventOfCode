#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <queue>
#include <map>
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

auto compare(int a, int b)
{
    return (a < b) ? -1 : ((a > b) ? 1 : 0);
}

auto do_part1()
{
    std::queue<long long> input, output;
    Computer c(std::string("input.txt"), input, output);
    auto blocks = 0;

    while (!c.halted)
    {
        c.execute_program_until_blocked();

        while (!output.empty())
        {
            auto x = int(output.front()); output.pop();
            auto y = int(output.front()); output.pop();
            auto value = output.front();  output.pop();

            if (value == 2)
                blocks++;
        }
    }

    return blocks;
}

auto do_part2()
{
    auto score = 0LL;

    std::queue<long long> input, output;
    Computer c(std::string("input.txt"), input, output);
    c.data[0] = 2;

    while (!c.halted)
    {
        c.execute_program_until_blocked();

        auto ball_x = 0, paddle_x = 0;

        while (!output.empty())
        {
            auto x = int(output.front()); output.pop();
            auto y = int(output.front()); output.pop();
            auto value = output.front();  output.pop();

            if (x == -1 && y == 0)
                score = value;
            else
            {
                if (value == 3)
                    paddle_x = x;
                else if (value == 4)
                    ball_x = x;
            }
        }

        input.push(compare(ball_x, paddle_x));
    }

    return score;
}

int main()
{
    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 363);
    assert(part2 == 17159);
    return 0;
}
