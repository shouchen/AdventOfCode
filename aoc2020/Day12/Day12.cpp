#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>

using IntPair = std::pair<int, int>;

auto navigate(const std::string &filename, IntPair &location, IntPair &direction, IntPair &nsew)
{
    std::ifstream file(filename);
    auto c = ' ';
    auto n = 0;

    while (file >> c >> n)
    {
        switch (c)
        {
        case 'N':
            nsew.first -= n;
            break;
        case 'S':
            nsew.first += n;
            break;
        case 'E':
            nsew.second += n;
            break;
        case 'W':
            nsew.second -= n;
            break;
        case 'L':
            for (auto i = 0; i < n; i += 90)
                direction = std::make_pair(-direction.second, direction.first);
            break;
        case 'R':
            for (auto i = 0; i < n; i += 90)
                direction = std::make_pair(direction.second, -direction.first);
            break;
        case 'F':
            location.first += direction.first * n;
            location.second += direction.second * n;
            break;
        }
    }

    return abs(location.first) + abs(location.second);
}

auto do_part1(const std::string &filename)
{
    auto location = std::make_pair(0, 0), direction = std::make_pair(0, 1);
    return navigate(filename, location, direction, location);
}

auto do_part2(const std::string &filename)
{
    auto location = std::make_pair(0, 0), waypoint = std::make_pair(-1, 10);
    return navigate(filename, location, waypoint, waypoint);
}

int main()
{
    auto part1 = do_part1("input.txt");
    auto part2 = do_part2("input.txt");

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 2458);
    assert(part2 == 145117);
    return 0;
}
