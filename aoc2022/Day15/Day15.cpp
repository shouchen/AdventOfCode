#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

auto get_dist(const std::pair<int, int> &p1, const std::pair<int, int> &p2)
{
    return abs(p1.first - p2.first) + abs(p1.second - p2.second);
}

struct Node
{
    long long sx, sy;
    long long bx, by;
    long dist;
};

std::vector<Node> nodes;

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string sensor, at, closest, beacon, is;
    auto x = ' ', y = ' ', equal = '=', comma = '.', colon = ':';
    auto x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    while (file >> sensor >> at >> x >> equal >> x1 >> comma >> y >> equal >> y1 >> colon >>
           closest >> beacon >> is >> at >> x >> equal >> x2 >> comma >> y >> equal >> y2)
    {
        std::pair<long long, long long> sens = std::make_pair(x1, y1), beac = std::make_pair(x2, y2);

        Node n{ x1,y1,x2,y2, get_dist(sens, beac) };
        nodes.push_back(n);
    }

    // find leftmost sensor and rightmost. Count will be leftmost - (dist - 1) to rightmost + (dist - 1)
    auto leftmost = LLONG_MAX, rightmost = LLONG_MIN;
    for (auto &n : nodes)
    {
        auto left = n.sx - n.dist + 1, right = n.sx + n.dist - 1;
        leftmost = std::min(leftmost, left), rightmost = std::max(rightmost, right);
    }

    auto retval = 0LL;
    auto row = 2000000LL;

    for (auto i = leftmost; i <= rightmost; i++)
    {
        auto ok = true;
        for (auto &n : nodes)
        {
            if (n.bx == i && n.by == row)
                break;

            if (get_dist(std::make_pair(n.sx, n.sy), std::make_pair(i , row)) <= n.dist)
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
    {
        auto temp = get_dist(std::make_pair(n.sx, n.sy), std::make_pair(x, y));
        if (temp <= n.dist)
            return false;
    }

    return true;
}

auto do_part2(const std::string &filename, int grace)
{
    std::ifstream file(filename);
    std::string sensor, at, closest, beacon, is;
    auto x = 'x', y = 'y', equal = '=', comma = ',', colon = ':';
    auto x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    while (file >> sensor >> at >> x >> equal >> x1 >> comma >> y >> equal >> y1 >> colon >>
        closest >> beacon >> is >> at >> x >> equal >> x2 >> comma >> y >> equal >> y2)
    {
        std::pair<long long, long long> sens = std::make_pair(x1, y1), beac = std::make_pair(x2, y2);

        Node n{ x1,y1,x2,y2, get_dist(sens, beac) };
        nodes.push_back(n);
    }

    // walk along edges of boundaries to find the one that works
    for (auto &n : nodes)
    {
        // top right diamond
        for (auto x = n.sx, y = n.sy - (n.dist + 1); y <= n.sy; x++, y++)
            if (x >= 0 && y >= 0 && x <= grace && y <= grace && test(x, y))
                return 4000000LL * x + y;

        // bottom right diamond
        for (auto x = n.sx + (n.dist + 1), y = n.sy; x >= n.sx; x--, y++)
            if (x >= 0 && y >= 0 && x <= grace && y <= grace && test(x, y))
                return 4000000LL * x + y;

        // bottom left diamond
        for (auto x = n.sx, y = n.sy + (n.dist + 1); y >= n.sy; x--, y--)
            if (x >= 0 && y >= 0 && x <= grace && y <= grace && test(x, y))
                return 4000000LL * x + y;

        // top left diamond
        for (auto x = n.sx - (n.dist + 1), y = n.sy; x <= n.sx; x++, y--)
            if (x >= 0 && y >= 0 && x <= grace && y <= grace && test(x, y))
                return 4000000LL * x + y;
    }

    return -1LL;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt", 4000000);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 4886370);
    assert(part2 == 11374534948438);
    return 0;
}
