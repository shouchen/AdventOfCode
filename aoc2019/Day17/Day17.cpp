#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <string>
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

std::vector<std::vector<char>> grid;
std::pair<int, int> loc, dir;

void read_input(const std::string &filename)
{
    const std::pair<int, int> dirs[] { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    std::queue<long long> input, output;
    Computer c(std::string(filename), input, output);

    c.execute_program_until_blocked();
    grid.push_back(std::vector<char>());

    while (!output.empty())
    {
        auto a = char(output.front()); output.pop();

        if (a == '\n' && grid.back().size())
        {
            grid.push_back(std::vector<char>());
        }
        else
        {
            grid.back().push_back(a);

            auto arrow = std::string("^v<>").find(a);
            if (arrow != std::string::npos)
            {
                loc = std::make_pair(grid.size() - 1, grid.back().size() - 1);
                dir = dirs[arrow];
            }
        }
    }

    grid.pop_back();
}

bool try_turn(std::stringstream &ss, char turn_direction)
{
    auto next_dir = (turn_direction == 'L')
        ? std::make_pair(-dir.second, dir.first)
        : std::make_pair(dir.second, -dir.first);
    auto next_loc = std::make_pair(loc.first + next_dir.first, loc.second + next_dir.second);

    if (next_loc.first < 0 || next_loc.first >= int(grid.size()) ||
        next_loc.second < 0 || next_loc.second >= int(grid[next_loc.first].size()) ||
        grid[next_loc.first][next_loc.second] != '#')
    {
        return false;
    }

    dir = next_dir;
    ss << turn_direction << ',';
    return true;
}

std::string determine_moves()
{
    std::stringstream ss;

    for (;;)
    {
        // Try going straight
        auto count = 0;
        while (loc.first + dir.first >= 0 && loc.first + dir.first < int(grid.size()) &&
            loc.second + dir.second >= 0 && loc.second + dir.second < int(grid[loc.first].size()) &&
            grid[loc.first + dir.first][loc.second + dir.second] == '#')
        {
            loc.first += dir.first, loc.second += dir.second;
            count++;
        }

        if (count)
            ss << count << ",";

        // Can't go straight anymore, so turn if possible and keep going
        if (!try_turn(ss, 'L') && !try_turn(ss, 'R'))
            break;
    }

    auto retval = ss.str();
    retval.pop_back();
    return retval;
}

void add_input(std::queue<long long> &input, const std::string &s)
{
    for (auto c : s) input.push(c);
    input.push('\n');
}

auto do_part1()
{
    read_input("input.txt");

    auto total = 0;

    for (auto y = 1U; y < grid.size() - 1; y++)
        for (auto x = 1U; x < grid[y].size() - 1; x++)
        {
            if (grid[y][x] == '#' &&
                grid[y - 1][x] == '#' && grid[y + 1][x] == '#' &&
                grid[y][x - 1] == '#' && grid[y][x + 1] == '#')
            {
                total += x * y;
            }
        }

    return total;
}

auto do_part2()
{
    auto moves = determine_moves();
    std::cout << moves << std::endl;

    /*
    Look at output from above and try to group into repeating groups:
    R,6,L,10,R,10,R,10,
    L,10,L,12,R,10,
    R,6,L,10,R,10,R,10,
    L,10,L,12,R,10,
    R,6,L,10,R,10,R,10,
    R,6,L,12,L,10,
    R,6,L,10,R,10,R,10,
    R,6,L,12,L,10,
    L,10,L,12,R,10,
    R,6,L,12,L,10

    Label each repeated group with A/B/C:
    A: R,6,L,10,R,10,R,10
    B: L,10,L,12,R,10
    A: R,6,L,10,R,10,R,10
    B: L,10,L,12,R,10
    A: R,6,L,10,R,10,R,10
    C: R,6,L,12,L,10
    A: R,6,L,10,R,10,R,10
    C: R,6,L,12,L,10
    B: L,10,L,12,R,10
    C: R,6,L,12,L,10

    Computer input:
    A,B,A,B,A,C,A,C,B,C
    R,6,L,10,R,10,R,10
    L,10,L,12,R,10
    R,6,L,12,L,10
    n
    */

    std::queue<long long> input, output;
    Computer computer(std::string("input.txt"), input, output);
    computer.data[0] = 2;

    add_input(input, "A,B,A,B,A,C,A,C,B,C");
    add_input(input, "R,6,L,10,R,10,R,10");
    add_input(input, "L,10,L,12,R,10");
    add_input(input, "R,6,L,12,L,10");
    add_input(input, "n");

    computer.execute_program_until_blocked();
    return output.back();
}

int main()
{
    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 3660);
    assert(part2 == 962913);
    return 0;
}
