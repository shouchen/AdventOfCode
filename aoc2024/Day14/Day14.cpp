#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

struct Position { int x, y; };
struct Velocity { int x, y; };

struct Robot
{
    Position p;
    Velocity v;
};

using Robots = std::vector<Robot>;

constexpr auto width = 101, height = 103;

auto compute_safety_factor(const Robots &robots)
{
    auto q1 = 0, q2 = 0, q3 = 0, q4 = 0;

    for (auto &r : robots)
    {
             if ((r.p.x < width / 2) && (r.p.y < height / 2)) q1++;
        else if ((r.p.x < width / 2) && (r.p.y > height / 2)) q2++;
        else if ((r.p.x > width / 2) && (r.p.y < height / 2)) q3++;
        else if ((r.p.x > width / 2) && (r.p.y > height / 2)) q4++;
    }

    return q1 * q2 * q3 * q4;
}

// For part 2, a manual process to visually the Christmas tree is too tedious. If we assume the
// robots are not overlapping when they for the Christmas tree, it is easy to find. If this
// assumption is wrong, we'll instead need to programmatically look at the entropy of each
// section of the robot system and identify a second when it drastically drops (due to the
// linearity and symmetry of the pattern. Fortunately, the earlier assumption proved to be true
// for our input data so the latter turned out to be unnecessary.

auto has_overlaps(const Robots &robots)
{
    for (auto y = 0; y < height; y++)
        for (auto x = 0; x < width; x++)
        {
            auto count = 0;
            for (auto &i : robots)
                if (i.p.x == x && i.p.y == y)
                    if (++count > 1)
                        return true;
        }

    return false;
}

void dump(const Robots &robots)
{
    std::cout << std::endl;
    for (auto y = 0; y < height; y++)
    {
        for (auto x = 0; x < width; x++)
        {
            auto count = 0;
            for (auto &r : robots)
                if (r.p.x == x && r.p.y == y)
                    count++;
            std::cout << (count ? '#' : '.');
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    Robots robots;
    auto p = 'p', v = 'v', equals = '=', comma = ',';
    auto px = 0, py = 0, vx = 0, vy = 0;
    auto retval = std::make_pair(0, 0);

    while (file >> p >> equals >> px >> comma >> py >> v >> equals >> vx >> comma >> vy)
        robots.push_back({ { px, py }, { vx, vy } });

    for (auto seconds = 1; !retval.first || !retval.second; seconds++)
    {
        for (auto &r : robots)
        {
            r.p.x += r.v.x, r.p.y += r.v.y;

            r.p.x = ((r.p.x % width) + width) % width;
            r.p.y = ((r.p.y % height) + height) % height;
        }

        if (seconds == 100)
            retval.first = compute_safety_factor(robots);

        if (!has_overlaps(robots))
        {
            retval.second = seconds;
            dump(robots);
        }
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl << std::endl;

    assert(answer.first == 231221760);
    assert(answer.second == 6771);
    return 0;
}
