#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

void update_ss(int &ss, int cycle, int x)
{
    if ((cycle + 20) % 40 == 0)
        ss += cycle * x;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string instruction;
    auto operand = 0, ss = 0, cycle = 1, x = 1;

    while (file >> instruction)
    {
        update_ss(ss, cycle++, x);
        if (instruction == "addx")
        {
            update_ss(ss, cycle++, x);
            file >> operand;
            x += operand;
        }
    }

    return ss;
}

void update_screen(std::string &screen, int crt, int x)
{
    auto temp = crt % 40;

    if (x == temp - 1 || x == temp || x == temp + 1)
        screen[crt] = '#';
}

auto format_screen(const std::string &screen)
{
    std::stringstream ss;

    for (auto i = 0; i < 240; i++)
    {
        if (i % 40 == 0)
            ss << std::endl;
        ss << screen[i];
    }

    return ss.str();
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string instruction;
    auto operand = 0, crt = 0, x = 1;
    std::string screen(240, '.');

    while (file >> instruction)
    {
        update_screen(screen, crt++, x);
        if (instruction == "addx")
        {
            update_screen(screen, crt++, x);
            file >> operand;
            x += operand;
        }
    }

    return format_screen(screen);
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 13180);
    // part2 displays "EZFCHJAB" in ASCII art
    return 0;
}
