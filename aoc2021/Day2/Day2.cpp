#include <iostream>
#include <fstream>
#include <cassert>

auto do_part(const std::string &filename, bool part2)
{
    auto pos = 0, depth = 0, aim = 0, n = 0;

    std::ifstream file(filename);
    std::string s;

    while (file >> s >> n)
    {
        if (s == "forward") pos += n, depth += aim * n;
        else if (s == "up") { part2 ? aim -= n : depth -= n; }
        else if (s == "down") { part2 ? aim += n : depth += n; }
    }

    return pos * depth;
}

int main()
{
    auto part1 = do_part("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 1694130);
    assert(part2 == 1698850445);
    return 0;
}
