#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

enum Opcode { Add = 1, Multiply = 2, Input = 3, Output = 4, JumpIfTrue = 5, JumpIfFalse = 6, LessThan = 7, Equals = 8, Halt = 99 };

std::vector<int> data;
auto pc = 0, inputval = 0, outputval = 0;

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

auto execute_instruction()
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
        get_param1() = inputval;
        pc += 2;
        break;
    case Output:
        outputval = get_param1();
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
        return false;
    }
    return true;
}

auto execute_program(int input)
{
    load_program("input.txt");
    inputval = input;
    pc = outputval = 0;

    while (execute_instruction());
    return outputval;
}

int main()
{
    auto part1 = execute_program(1);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = execute_program(5);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 4601506);
    assert(part2 == 5525561);
    return 0;
}
