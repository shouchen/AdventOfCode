#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

auto x = 1;

auto inc_cycle_and_check(int &cycle)
{
    ++cycle;
    return ((cycle + 20) % 40 == 0);
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, instr;
    auto operand = 0, cycle = 0, ss = 0;

    while (file >> instr)
    {
        if (inc_cycle_and_check(cycle))
            ss += cycle * x;

        if (instr == "noop")
            continue;

        if (instr == "addx")
        {
            file >> operand;
            if (inc_cycle_and_check(cycle))
                ss += cycle * x;

            x += operand;
        }
    }

    return ss;
}

std::string screen = "................................................................................................................................................................................................................................................";

void inc_crt_and_check(int &crt, int cycle)
{
    cycle++;

    auto temp = crt % 40;

    if (x == temp - 1 || x == temp || x == temp + 1)
        screen[crt] = '#';

    if (++crt == 240) crt = 0;
}

void do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, instr;
    auto operand = 0, crt = 0, cycle = 0;

    x = 1;

    while (file >> instr)
    {
        if (instr == "noop")
        {
            inc_crt_and_check(crt, cycle);
        }
        else if (instr == "addx")
        {
            file >> operand;
            inc_crt_and_check(crt, cycle);
            inc_crt_and_check(crt, cycle);
            x += operand;
        }
    }

    for (auto i = 0; i < 240; i++)
    {
        if (i % 40 == 0)
            std::cout << std::endl;
        std::cout << screen[i];
    }
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    std::cout << "Part Two: ";
    do_part2("input.txt");

    assert(part1 == 13180);
    // part2 displays "EZFCHJAB"
    return 0;
}
