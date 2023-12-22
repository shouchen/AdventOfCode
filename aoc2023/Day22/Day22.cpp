#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <cassert>

struct Point { int x, y, z; };
using Brick = std::pair<Point, Point>;

std::vector<Brick> bricks;
std::vector<std::vector<int>> vertically_adjacent;

void read_input(const std::string &filename)
{
    std::ifstream file("input.txt");
    auto c = ',';

    Brick brick;
    while (file >> brick.first.x >> c >> brick.first.y >> c >> brick.first.z >> c
                >> brick.second.x >> c >> brick.second.y >> c >> brick.second.z)
        bricks.push_back(brick);
}

auto do_part1(const std::string &filename)
{
    sort(bricks.begin(), bricks.end(), [&](auto lhs, auto rhs) {
        return lhs.first.z < rhs.first.z;
    });

    auto max_xy_index = 0;
    for (auto &b : bricks)
    {
        max_xy_index = std::max(max_xy_index, b.first.x);
        max_xy_index = std::max(max_xy_index, b.second.x);
        max_xy_index = std::max(max_xy_index, b.first.y);
        max_xy_index = std::max(max_xy_index, b.second.y);
    }

    std::vector<std::vector<int>> max_height_at_xy(max_xy_index + 1, std::vector<int>(max_xy_index + 1));
    std::vector<std::vector<int>> index(max_xy_index + 1, std::vector<int>(max_xy_index + 1, -1));

    std::set<int> unsafe_to_disintegrate;
    vertically_adjacent = std::vector<std::vector<int>>(bricks.size());

    for (auto i = 0; i < bricks.size(); i++)
    {
        auto &curr = bricks[i];
        auto x1 = curr.first.x, y1 = curr.first.y;
        auto x2 = curr.second.x, y2 = curr.second.y;

        auto max_height_under_curr = 0;
        for (auto x = x1; x <= x2; x++)
            for (auto y = y1; y <= y2; y++)
                max_height_under_curr = std::max(max_height_under_curr, max_height_at_xy[x][y]);

        std::set<int> bricks_directly_under_current;
        for (auto x = x1; x <= x2; x++)
            for (auto y = y1; y <= y2; y++)
            {
                if (index[x][y] != -1 && max_height_at_xy[x][y] == max_height_under_curr)
                    bricks_directly_under_current.insert(index[x][y]);

                max_height_at_xy[x][y] = max_height_under_curr + (curr.second.z - curr.first.z + 1);
                index[x][y] = i;
            }

        if (bricks_directly_under_current.size() == 1)
            unsafe_to_disintegrate.insert(*bricks_directly_under_current.begin());

        for (auto j : bricks_directly_under_current)
            vertically_adjacent[j].push_back(i);
    }

    return bricks.size() - unsafe_to_disintegrate.size();
}

auto do_part2()
{
    auto retval = 0;

    for (auto i = 0; i < bricks.size(); i++)
    {
        std::vector<int> degree(bricks.size(), 0);

        for (auto j = 0; j < bricks.size(); j++)
            for (auto k : vertically_adjacent[j])
                degree[k]++;

        std::queue<int> q({ i });

        while (!q.empty())
        {
            auto item = q.front(); q.pop();
            if (item != i)
                retval++;

            for (auto j : vertically_adjacent[item])
                if (--degree[j] == 0)
                    q.push(j);
        }
    }

    return retval;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 398);
    assert(part2 == 70727);
    return 0;
}