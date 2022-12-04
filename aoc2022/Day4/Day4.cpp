#include <iostream>
#include <fstream>
#include <cassert>

auto part1_overlap(int a, int b, int c, int d)
{
    return (c >= a && d <= b) || (a >= c && b <= d);
}

auto part2_extra_overlap(int a, int b, int c, int d)
{
    return
        (c <= a && d >= a) || (c <= b && d >= b) ||
        (a <= c && b >= c) || (a <= d && b >= d);
}

void process_input(const std::string &filename, int &part1, int &part2)
{
    std::ifstream file(filename);
    auto a = 0, b = 0, c = 0, d = 0;
    auto dash = '-', comma = ',';

    while (file >> a >> dash >> b >> comma >> c >> dash >> d)
        if (part1_overlap(a, b, c, d))
            part1++, part2++;
        else if (part2_extra_overlap(a, b, c, d))
            part2++;
}

int main()
{
    auto part1 = 0, part2 = 0;
    process_input("input.txt", part1, part2);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 602);
    assert(part2 == 891);
    return 0;
}