#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

struct Node
{
    int sx, sy;
    int bx, by;
    int dist;
};

std::vector<Node> nodes;

inline auto get_dist(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string sensor, at, closest, beacon, is;
    auto x = ' ', y = ' ', equals = '=', comma = ',', colon = ':';
    auto x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    while (file >> sensor >> at >> x >> equals >> x1 >> comma >> y >> equals >> y1 >> colon >>
        closest >> beacon >> is >> at >> x >> equals >> x2 >> comma >> y >> equals >> y2)
    {
        nodes.push_back(Node{ x1, y1, x2, y2, get_dist(x1, y1, x2, y2) });
    }
}

auto do_part1(const std::string &filename, int row)
{
    // find leftmost sensor and rightmost. Count will be leftmost - (dist - 1) to rightmost + (dist - 1)
    auto leftmost = INT_MAX, rightmost = INT_MIN;
    for (auto &n : nodes)
    {
        auto left = n.sx - n.dist + 1, right = n.sx + n.dist - 1;
        leftmost = std::min(leftmost, left), rightmost = std::max(rightmost, right);
    }

    auto retval = 0;

    for (auto i = leftmost; i <= rightmost; i++)
    {
        auto ok = true;
        for (auto &n : nodes)
        {
            if (n.bx == i && n.by == row)
                break;

            if (get_dist(n.sx, n.sy, i, row) <= n.dist)
            {
                ok = false;
                break;
            }
        }

        if (!ok)
            retval++;
    }

    return retval;
}

auto test(int x, int y)
{
    // See if for any node, this point is closer than their closest beacon
    for (auto &n : nodes)
        if (get_dist(n.sx, n.sy, x, y) <= n.dist)
            return false;

    return true;
}

auto do_part2(const std::string &filename, int grace)
{
    // walk diamond along edges of boundaries to find the one that works
    for (auto &n : nodes)
    {
        // top right diamond
        for (auto x = n.sx, y = n.sy - (n.dist + 1); y < n.sy; x++, y++)
            if (x >= 0 && y >= 0 && x <= grace && y <= grace && test(x, y))
                return 4000000LL * x + y;

        // bottom right diamond
        for (auto x = n.sx + (n.dist + 1), y = n.sy; x > n.sx; x--, y++)
            if (x >= 0 && y >= 0 && x <= grace && y <= grace && test(x, y))
                return 4000000LL * x + y;

        // bottom left diamond
        for (auto x = n.sx, y = n.sy + (n.dist + 1); y > n.sy; x--, y--)
            if (x >= 0 && y >= 0 && x <= grace && y <= grace && test(x, y))
                return 4000000LL * x + y;

        // top left diamond
        for (auto x = n.sx - (n.dist + 1), y = n.sy; x < n.sx; x++, y--)
            if (x >= 0 && y >= 0 && x <= grace && y <= grace && test(x, y))
                return 4000000LL * x + y;
    }

    return -1LL;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part1("input.txt", 2000000);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt", 4000000);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 4886370);
    assert(part2 == 11374534948438);
    return 0;
}
