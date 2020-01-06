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

bool test_cell(unsigned row, unsigned col)
{
    std::queue<long long> input, output;
    Computer c("input.txt", input, output);

    input.push(col);
    input.push(row);
    c.execute_program_until_blocked();
    return output.front() == 1;
}

auto do_part1()
{
    auto count = 0U;

    for (auto i = 0; i < 50; i++)
        for (auto j = 0; j < 50; j++)
            if (test_cell(i, j))
                count++;

    return count;
}

// Initialize 2nd and 3rd params to those of prev row before calling. They'll
// return the adjusted values for the new row underneath.
void get_start_end_cols(int row, int &start_col, int &end_col)
{
    for (; !test_cell(row, start_col); start_col++);
    for (end_col = std::max(end_col, start_col); test_cell(row, end_col + 1); end_col++);
}

auto do_part2()
{
    const auto dim = 100;
    auto top_row = 0, top_end_col = 0, bottom_start_col = 0, bottom_end_col = 0;
    std::queue<int> recent_end_cols;

    for (auto bottom_row = 4; top_end_col - bottom_start_col + 1 < dim; bottom_row++)
    {
        get_start_end_cols(bottom_row, bottom_start_col, bottom_end_col);

        if (recent_end_cols.size() == dim)
        {
            recent_end_cols.pop();
            recent_end_cols.push(bottom_end_col);

            top_row = bottom_row - dim + 1;
            top_end_col = recent_end_cols.front();
        }
        else
            recent_end_cols.push(bottom_end_col);
    }

    return bottom_start_col * 10000 + top_row;
}

int main()
{
    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 171);
    assert(part2 == 9741242);
    return 0;
}
