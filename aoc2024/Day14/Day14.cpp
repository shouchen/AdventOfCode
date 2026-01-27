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

std::vector<Robot> robots;
const auto width = 101, height = 103;

void dump()
{
    for (auto y = 0; y < height; y++)
    {
        for (auto x = 0; x < width; x++)
        {
            auto count = 0;
            for (auto &i : robots)
                if (i.p.x == x && i.p.y == y)
                    count++;
            std::cout << (count ? char(count + '0') : '.');
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool no_overlaps()
{
    for (auto y = 0; y < height; y++)
        for (auto x = 0; x < width; x++)
        {
            auto count = 0;
            for (auto &i : robots)
                if (i.p.x == x && i.p.y == y)
                    if (++count > 1) return false;
        }

    return true;
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
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

            while (r.p.x < 0)
                r.p.x += width;
            while (r.p.x >= width)
                r.p.x -= width;
            while (r.p.y < 0)
                r.p.y += height;
            while (r.p.y >= height)
                r.p.y -= height;
        }

        if (seconds == 100)
        {
            auto q1 = 0, q2 = 0, q3 = 0, q4 = 0;

            for (auto &r : robots)
            {
                if ((r.p.x < width / 2) && (r.p.y < height / 2))
                    q1++;
                else if ((r.p.x < width / 2) && (r.p.y > height / 2))
                    q2++;
                else if ((r.p.x > width / 2) && (r.p.y < height / 2))
                    q3++;
                else if ((r.p.x > width / 2) && (r.p.y > height / 2))
                    q4++;
            }

            retval.first = q1 * q2 * q3 * q4;
        }

        if (no_overlaps())
        {
            retval.second = seconds;
            std::cout << std::endl;
            dump();
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
