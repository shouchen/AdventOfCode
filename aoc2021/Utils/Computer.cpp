#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <cassert>

std::vector<std::pair<std::string, int>> program;
int pc = 0, acc = 0;

void read_program(const std::string& filename)
{
    std::ifstream file("input.txt");
    std::string(opcode);
    auto n = 0;

    while (file >> opcode >> n)
        program.push_back(std::make_pair(opcode, n));
}

bool execute(int repair = -1)
{
    acc = 0;
    std::set<int> visited;

    for (pc = 0; pc < int(program.size()); pc++)
    {
        if (visited.find(pc) != visited.end())
            return false;

        visited.insert(pc);

        std::string opcode = program[pc].first;
        auto n = program[pc].second;

        if (pc == repair)
            if (opcode == "nop")
                opcode = "jmp";
            else if (opcode == "jmp")
                opcode = "nop";

        if (opcode == "acc")
            acc += n;
        else if (opcode == "jmp")
            pc += n - 1;
    }

    return true;
}

auto do_part1(const std::string& filename)
{
    execute();
    return acc;
}

auto do_part2()
{
    for (auto i = 0U; i < program.size(); i++)
        if (execute(i))
            return acc;

    return -1;
}

int main()
{
    read_program("input.txt");

    auto part1 = do_part1("input.txt");
    auto part2 = do_part2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 1801);
    assert(part2 == 2060);
    return 0;
}
