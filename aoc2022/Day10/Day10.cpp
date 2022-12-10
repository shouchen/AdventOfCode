#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>

void do_cycle(int x, int &cycle, int &ss, std::string &screen)
{
    if (cycle % 40 == 20)
        ss += cycle * x;

    auto crt = cycle - 1;
    screen.push_back((abs(crt % 40 - x) < 2) ? '#' : '.');

    if (cycle++ % 40 == 1)
        screen.push_back('\n');
}

auto process_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string instruction, screen;
    auto operand = 0, ss = 0, cycle = 1, x = 1;

    while (file >> instruction)
    {
        do_cycle(x, cycle, ss, screen);

        if (instruction == "addx")
        {
            do_cycle(x, cycle, ss, screen);

            file >> operand;
            x += operand;
        }
    }

    return std::make_pair(ss, screen);
}

int main()
{
    auto output = process_input("input.txt");
    std::cout << "Part One: " << output.first << std::endl;
    std::cout << "Part Two: " << output.second << std::endl;

    assert(output.first == 13180);
    // part2 displays "EZFCHJAB" in ASCII art
    return 0;
}
