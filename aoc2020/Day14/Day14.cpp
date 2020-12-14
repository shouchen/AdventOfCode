#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cassert>

std::map<unsigned long long, unsigned long long> mem;

void write_memory(unsigned long long addr, unsigned long long value, unsigned long long floats = 0)
{
    if (floats)
    {
        auto first_float = floats & (~floats + 1);
        floats &= ~first_float;

        write_memory(addr & ~first_float, value, floats);
        write_memory(addr | first_float, value, floats);
    }
    else
        return (void)(mem[addr] = value);
}

auto do_part(const std::string &filename, bool part2)
{
    mem.clear();

    std::ifstream file(filename);
    std::string s;
    auto zeros = 0ULL, ones = 0ULL, exes = 0ULL;

    while (std::getline(file, s))
    {
        if (s.substr(0, 7) == "mask = ")
        {
            zeros = ones = exes = 0ULL;

            for (auto b = s.begin() + 7; b != s.end(); b++)
            {
                zeros = (zeros << 1) | (*b == '0' ? 1 : 0);
                ones = (ones << 1) | (*b == '1' ? 1 : 0);
                exes = (exes << 1) | (*b == 'X' ? 1 : 0);
            }
        }
        else
        {
            auto addr = atoi(s.c_str() + 4);
            auto value = atoi(s.c_str() + s.find(" = ") + 3);

            if (part2)
                write_memory(addr | ones, value, exes);
            else
                write_memory(addr, value & ~zeros | ones);
        }
    }

    auto sum = 0ULL;
    for (auto &a : mem)
        sum += a.second;

    return sum;
}

int main()
{
    auto part1 = do_part("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 11612740949946);
    assert(part2 == 3394509207186);
    return 0;
}
