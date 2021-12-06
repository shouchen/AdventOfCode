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
        auto spawning = fish_count[0];

        for (auto i = 0; i < 8; i++)
            fish_count[i] = fish_count[i + 1];

        fish_count[6] += spawning;
        fish_count[8] = spawning;
    }

    auto count = 0ULL;
    for (auto i : fish_count)
        count += i;

    return count;
}

// For more optimization with less readability, it's possible to entirely avoid
// the array rotations by replacing the while loop with this:
// 
// auto a = 0, b = 7;
// while (days--)
// {
//     fish_count[b] += fish_count[a];
//     if (++a == 9) a = 0;
//     if (++b == 9) b = 0;
// }

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
