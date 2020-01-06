#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <cassert>

enum Opcode { Add = 1, Multiply = 2, Input = 3, Output = 4, JumpIfTrue = 5, JumpIfFalse = 6, LessThan = 7, Equals = 8, Halt = 99 };

struct Computer
{
    std::vector<int> data;
    int pc = 0;
    bool halted = false;

    Opcode get_instruction() { return Opcode(data[pc] % 100); }
    auto &get_param1() { return ((data[pc] / 100) % 10) ? data[pc + 1] : data[data[pc + 1]]; }
    auto &get_param2() { return ((data[pc] / 1000) % 10) ? data[pc + 2] : data[data[pc + 2]]; }
    auto &get_param3() { return ((data[pc] / 10000) % 10) ? data[pc + 3] : data[data[pc + 3]]; }

    void load_program(const std::string &filename)
    {
        data.clear();

        std::ifstream file(filename);
        auto a = 0;
        auto comma = ',';

        while (file >> a)
        {
            data.push_back(a);
            file >> comma;
        }
    }

    bool execute_instruction(std::queue<int> &inputvals, std::queue<int> &outputvals)
    {
        switch (get_instruction())
        {
        case Add:
            get_param3() = get_param1() + get_param2();
            pc += 4;
            break;
        case Multiply:
            get_param3() = get_param1() * get_param2();
            pc += 4;
            break;
        case Input:
            if (inputvals.empty())
                return false;
            get_param1() = inputvals.front();
            inputvals.pop();
            pc += 2;
            break;
        case Output:
            outputvals.push(get_param1());
            pc += 2;
            break;
        case JumpIfTrue:
            pc = (get_param1() != 0) ? get_param2() : (pc + 3);
            break;
        case JumpIfFalse:
            pc = (get_param1() == 0) ? get_param2() : (pc + 3);
            break;
        case LessThan:
            get_param3() = (get_param1() < get_param2()) ? 1 : 0;
            pc += 4;
            break;
        case Equals:
            get_param3() = (get_param1() == get_param2()) ? 1 : 0;
            pc += 4;
            break;
        default:
            assert(false);
        case Halt:
            halted = true;
            return false;
        }
        return true;
    }

    void load_program()
    {
        load_program("input.txt");
        pc = 0;
    }

    void execute_program_until_blocked(std::queue<int> &inputvals, std::queue<int> &outputvals)
    {
        while (execute_instruction(inputvals, outputvals));
    }
};

auto try_perm(const std::vector<int> &phase)
{
    const auto num_computers = phase.size();
    std::vector<Computer> computer(num_computers);
    std::vector<std::queue<int>> pipe(num_computers);

    for (auto i = 0U; i < num_computers; i++)
    {
        computer[i].load_program();
        pipe[i].push(phase[i]);
    }

    pipe[0].push(0);

    while (!computer[num_computers - 1].halted)
        for (auto i = 0U; i < num_computers; i++)
            computer[i].execute_program_until_blocked(pipe[i], pipe[(i + 1) % num_computers]);

    return pipe[0].front();
}

auto find_maximum(std::vector<int> phases)
{
    auto maximum = INT32_MIN;
    do
    {
        maximum = std::max(maximum, try_perm(phases));
    } while (std::next_permutation(phases.begin(), phases.end()));

    return maximum;
}

int main()
{
    auto part1 = find_maximum({ 0, 1, 2, 3, 4 });
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = find_maximum({ 5, 6, 7, 8, 9 });
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 272368);
    assert(part2 == 19741286);
    return 0;
}
