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

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
