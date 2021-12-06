#include <iostream>
#include <fstream>
#include <cassert>

auto do_part(const std::string &filename, unsigned days)
{
    unsigned long long fish_count[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    std::ifstream file(filename);
    auto n = 0;
    auto comma = ',';

    while (file >> n)
    {
        fish_count[n]++;
        file >> comma;
    }

    while (days--)
    {
        auto temp = fish_count[0];
        fish_count[0] = fish_count[1];
        fish_count[1] = fish_count[2];
        fish_count[2] = fish_count[3];
        fish_count[3] = fish_count[4];
        fish_count[4] = fish_count[5];
        fish_count[5] = fish_count[6];
        fish_count[6] = fish_count[7] + temp;
        fish_count[7] = fish_count[8];
        fish_count[8] = temp;
    }

    auto count = 0ULL;
    for (auto i : fish_count)
        count += i;

    return count;
}

int main()
{
    auto part1 = do_part("input.txt", 80);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part("input.txt", 256);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 390011);
    assert(part2 == 1746710169834);
    return 0;
}
