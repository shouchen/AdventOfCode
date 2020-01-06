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

using XY = std::pair<int, int>;

std::queue<long long> input, output;
Computer c(std::string("input.txt"), input, output);

std::map<XY, int> grid; // dist from origin, -1 means wall, -2 means target

std::vector<std::pair<int, int>> dirns{{ 0, 0 }, { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 }}; // xNSWE

XY target;
auto target_dist = -1;

int recur(int x, int y)
{
    auto dist = grid[std::make_pair(x, y)];

    for (auto i = 1; i <= 4; i++)
    {
        auto xdir = dirns[i].first, ydir = dirns[i].second;

        if (grid.find(std::make_pair(x + xdir, y + ydir)) != grid.end())
            continue;

        input.push(i);
        c.execute_program_until_blocked();
        auto result = output.back(); output.pop();

        if (result == 0)
        {
            grid[std::make_pair(x + xdir, y + ydir)] = -1;
            continue;
        }
        else if (result == 2)
        {
            target = std::make_pair(x + xdir, y + ydir);
            target_dist = dist + 1;
        }

        grid[std::make_pair(x + xdir, y + ydir)] = dist + 1;

        auto retval = recur(x + xdir, y + ydir);
        if (retval > 0)
            return retval;

        auto go_back_dir = ((i - 1) ^ 1) + 1;
        input.push(go_back_dir);

        c.execute_program_until_blocked();
        result = output.back(); output.pop();

        assert(result == 1);
    }

    return 0;
}

auto do_part1_dfs()
{
    auto x = 0, y = 0, dist = 0;
    grid[std::make_pair(x, y)] = dist;

    recur(x, y);
    return target_dist;
}

auto do_part2() // uses grid from part1, and the discovered target location
{
    std::map<std::pair<int, int>, int> dist_from_target;
    dist_from_target[target] = 0;

    std::queue<std::pair<int, int>> q;
    q.push(target);

    auto retval = -1;

    while (!q.empty())
    {
        auto location = q.front(); q.pop();

        for (auto i = 1; i <= 4; i++)
        {
            auto xdir = dirns[i].first, ydir = dirns[i].second;
            auto next_location = std::make_pair(location.first + xdir, location.second + ydir);

            assert(grid.find(next_location) != grid.end());

            auto temp = grid[next_location];
            if (temp != -1 && dist_from_target.find(next_location) == dist_from_target.end())
            {
                retval = dist_from_target[location] + 1;
                dist_from_target[next_location] = retval;
                q.push(next_location);
            }
        }
    }

    return retval;
}

int main()
{
    auto part1 = do_part1_dfs();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 330);
    assert(part2 == 352);
    return 0;
}
