#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <array>
#include <string>
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

    auto &get_param(unsigned n)
    {
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

// 1. Interactively explore the world and map it out on paper (what connects to
//    what, what objects there are, what the destination is). Do this by
//    uncommenting play_interactively() in main().
// 2. Try to pick up each object. Some immediately kill you, so note those:
//    - photons
//    - escape pod
//    - giant electromagnet
//    - molten lava
// 3. List out the other objects since the solution will involve some
//    combination of them.
std::vector<std::string> items
{
    "candy cane",
    "coin",
    "food ration",
    "mouse",
    "mug",
    "mutex",
    "ornament",
    "semiconductor"
};
// 4. Program the ASCII steps to pick up all non-fatal objects and bring them to
//    the destination (Security Checkpoint).
std::vector<std::string> commands =
{
    { "north"},  { "take mug"},
    { "north" }, { "take food ration" },
    { "south" }, { "east" },  { "north" }, { "east" },  { "take semiconductor" },
    { "west" },  { "south" }, { "west" },  { "south" }, { "east"}, { "take ornament" },
    { "north" }, { "take coin" },
    { "east" },  { "take mutex" },
    { "west" },  { "south" }, { "east" },  { "take candy cane"},
    { "west" },  { "west" },  { "south" }, { "east" },  { "take mouse" },
    { "south" }
};

std::queue<long long> input, output;
Computer c("input.txt", input, output);

void add_command(const std::string &command)
{
    for (auto c : command)
        input.push(c);
    input.push('\n');
}

std::string get_result()
{
    std::string result;
    while (!output.empty())
    {
        result.push_back(char(output.front()));
        output.pop();
    }

    return result;
}

void play_interactively()
{
    for (;;)
    {
        c.execute_program_until_blocked();
        std::cout << get_result();

        std::string command;
        std::getline(std::cin, command);
        add_command(command);
    }
}

void play_program()
{
    for (auto &command : commands)
        add_command(command);

    c.execute_program_until_blocked();
    get_result();
}

int main()
{
    // play_interactively();

    play_program();
    
    // You start holding all non-fatal objects, just try all combos of bitsets.
    const auto all_bits_set = (1 << items.size()) - 1;
    for (auto combo = all_bits_set, prev = all_bits_set; combo >= 0; prev = combo--)
    {
        for (auto j = 0U, bit_mask = 1U; j < items.size(); j++, bit_mask <<= 1)
        {
            auto prev_had = (prev & bit_mask) != 0;
            auto should_have = (combo & bit_mask) != 0;

            if (should_have && !prev_had)
                add_command("take " + items[j]);
            else if (!should_have && prev_had)
                add_command("drop " + items[j]);
        }

        add_command("west");
        c.execute_program_until_blocked();
        auto result = get_result();

        if (result.find("ejected back to the checkpoint") == std::string::npos)
        {
            std::cout << result;
            break;
        }
    }

    return 0;
}
