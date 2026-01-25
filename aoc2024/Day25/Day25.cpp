#include <iostream>
#include <fstream>
#include <cassert>

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
        ;

    return -1;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
        ;

    return -1;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    //assert(part1 == );

    std::cout << "Part Two: N/A" << std::endl;
    return 0;
}
