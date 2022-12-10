#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cassert>

void update_signal_strength(int &ss, int cycle, int x)
{
    if (cycle % 40 == 20)
        //if ((cycle + 20) % 40 == 0)
        ss += cycle * x;
}

void update_screen(std::string &screen, int crt, int x)
{
    if (abs(crt % 40 - x) < 2)
        screen[crt] = '#';
}

void process_input(const std::string &filename, int &part1, std::string &part2)
{
    std::ifstream file(filename);
    std::string instruction;
    auto operand = 0, ss = 0, cycle = 1, crt = 0, x = 1;
    std::string screen(240, '.');

    while (file >> instruction)
    {
        update_signal_strength(ss, cycle++, x);
        update_screen(screen, crt++, x);

        if (instruction == "addx")
        {
            update_signal_strength(ss, cycle++, x);
            update_screen(screen, crt++, x);

            file >> operand;
            x += operand;
        }
    }

    part1 = ss;
    std::stringstream ss2;

    for (auto i = 0; i < screen.length(); i++)
    {
        if (i % 40 == 0)
            ss2 << std::endl;
        ss2 << screen[i];
    }

    part2 = ss2.str();
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
