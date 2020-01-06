#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <array>
#include <set>
#include <memory>
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

auto do_either_part(bool part1)
{
    std::queue<long long> input[50], output[50];
    std::vector<std::unique_ptr<Computer>> computer;

    for (auto i = 0; i < 50; i++)
    {
        computer.push_back(std::make_unique<Computer>("input.txt", input[i], output[i]));
        input[i].push(i);
    }

    std::set<long long> nat_sent_y;
    auto nat_x = 0LL, nat_y = 0LL;

    for (;;)
    {
        auto something_sent = false, something_received = false;

        for (auto i = 0; i < 50; i++)
        {
            computer[i]->execute_program_until_blocked();

            if (input[i].empty())
                input[i].push(-1);
            else
                something_received = true;

            if (!output[i].empty())
            {
                something_sent = true;

                auto dest = output[i].front();
                output[i].pop();

                auto x = output[i].front();
                output[i].pop();

                auto y = output[i].front();
                output[i].pop();

                if (dest == 255)
                {
                    if (part1)
                        return y;

                    nat_x = x, nat_y = y;
                }
                else
                {
                    input[dest].push(x);
                    input[dest].push(y);
                }
            }
        }

        if (!something_sent && !something_received)
        {
            if (nat_sent_y.find(nat_y) != nat_sent_y.end())
                return nat_y;

            input[0].push(nat_x);
            input[0].push(nat_y);
            nat_sent_y.insert(nat_y);
        }
    }
}

int main()
{
    auto part1 = do_either_part(true);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_either_part(false);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 21089);
    assert(part2 == 16658);
    return 0;
}
