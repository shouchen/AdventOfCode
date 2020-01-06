#include "stdafx.h"
#include <iostream>
#include <fstream>
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

private:
    enum Opcode { Add = 1, Multiply = 2, Input = 3, Output = 4, JumpIfTrue = 5, JumpIfFalse = 6, LessThan = 7, Equals = 8, Rebase = 9, Halt = 99 };

    static const std::array<int, 4> param_shift;

    std::map<long long, long long> data;
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

enum Color { Black, White };
enum TurnDirection { Left, Right };

using XY = std::pair<int, int>;

XY &operator+=(XY &a, const XY &b)
{
    a.first += b.first, a.second += b.second;
    return a;
}

auto do_part(Color initial_color, bool dump_final_state)
{
    std::map<XY, Color> color;

    auto pos = std::make_pair(0, 0), dir = std::make_pair(0, -1);
    auto min = std::make_pair(INT_MAX, INT_MAX), max = std::make_pair(INT_MIN, INT_MIN);

    std::queue<long long> input, output;
    Computer c(std::string("input.txt"), input, output);

    color[pos] = initial_color;

    while (!c.halted)
    {
        input.push(color[pos]);
        c.execute_program_until_blocked();

        while (!output.empty())
        {
            auto color_to_paint = Color(output.front());
            output.pop();
            auto turn_direction = TurnDirection(output.front());
            output.pop();

            color[pos] = color_to_paint;

            min.first = std::min(pos.first, min.first), min.second = std::min(pos.second, min.second);
            max.first = std::max(pos.first, max.first), max.second = std::max(pos.second, max.second);

            dir = (turn_direction == Left)
                ? std::make_pair(dir.second, -dir.first)
                : std::make_pair(-dir.second, dir.first);

            pos += dir;
        }
    }

    if (dump_final_state)
    {
        for (auto y = min.second; y <= max.second; y++)
        {
            for (auto x = min.first; x < max.first; x++)
            {
                auto iter = color.find(std::make_pair(x, y));
                bool is_white = iter != color.end() && iter->second == White;
                std::cout << (is_white ? '#' : '.');
            }
            std::cout << std::endl;
        }
    }

    return color.size();
}

int main()
{
    auto part1 = do_part(Black, false);
    std::cout << "Part One: " << part1 << std::endl;

    std::cout << "Part Two: " << std::endl;
    do_part(White, true);

    assert(part1 == 2041);
    // Part2 visually spells out "ZRZPKEZR" in ASCII art
    return 0;
}
