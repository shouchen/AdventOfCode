#include <iostream>
#include <fstream>
#include <map>
#include <cassert>

auto do_part(const std::string &filename, bool part2)
{
    std::map<std::pair<int, int>, int> grid;
    std::ifstream file(filename);

    auto x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    char comma;
    std::string arrow;

    while (file >> x1 >> comma >> y1 >> arrow >> x2 >> comma >> y2)
    {
        if (x1 == x2 || y1 == y2 || part2)
        {
            auto xdir = (x1 == x2) ? 0 : ((x1 < x2) ? 1 : -1);
            auto ydir = (y1 == y2) ? 0 : ((y1 < y2) ? 1 : -1);
            auto xdist = abs(x1 - x2), ydist = abs(y1 - y2);
            auto dist = xdist ? xdist : ydist;

            for (auto i = 0; i <= dist; i++)
            {
                grid[std::make_pair(x1, y1)]++;
                x1 += xdir, y1 += ydir;
            }
        }
    }

    auto count = 0;
    for (auto &q : grid)
        if (q.second > 1)
            count++;

    return count;
}

int main()
{
    auto part1 = do_part("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 5294);
    assert(part2 == 21698);
    return 0;
}
