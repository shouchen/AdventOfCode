#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>

auto get_signal_strength(int cycle, int x)
{
    return (cycle % 40 == 20) ? (cycle * x) : 0;
}

auto get_pixel(int crt, int x)
{
    return (abs(crt % 40 - x) < 2) ? '#' : '.';
}

void do_cycle(int x, int &cycle, int &ss, int &crt, std::string &screen)
{
    ss += get_signal_strength(cycle++, x);
    screen.push_back(get_pixel(crt++, x));

    if (crt % 40 == 0)
        screen.push_back('\n');
}

void process_input(const std::string &filename, int &part1, std::string &part2)
{
    std::ifstream file(filename);
    std::string instruction, screen;
    auto operand = 0, ss = 0, cycle = 1, crt = 0, x = 1;

    while (file >> instruction)
    {
        do_cycle(x, cycle, ss, crt, screen);

        if (instruction == "addx")
        {
            do_cycle(x, cycle, ss, crt, screen);

            file >> operand;
            x += operand;
        }
    }

    part1 = ss;
    part2 = screen;
}

int main()
{
    auto part1 = 0;
    std::string part2;
    process_input("input.txt", part1, part2);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 13180);
    // part2 displays "EZFCHJAB" in ASCII art
    return 0;
}
