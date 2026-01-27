#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

using Position = std::pair<int, int>;
using Velocity = std::pair<int, int>;

//int width = 11, height = 7;
int width = 101, height = 103;

struct Robot
{
    Position p;
    Velocity v;
};

std::vector<Robot> robots;

void dump()
{
    for (auto y = 0; y < height; y++)
    {
        for (auto x = 0; x < width; x++)
        {
            auto count = 0;
            for (auto &i : robots)
                if (i.p.first == x && i.p.second == y)
                    count++;
            std::cout << (count ? char(count + '0') : '.');
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto p = 'p', v = 'v', equals = '=', comma = ',';
    auto px = 0, py = 0, vx = 0, vy = 0;

    while (file >> p >> equals >> px >> comma >> py >> v >> equals >> vx >> comma >> vy)
        robots.push_back({ { px, py }, { vx, vy } });
    //robots.push_back({ { 2,4, }, { 2,-3 } });

    //dump();

    for (auto seconds = 1; seconds <= 100; seconds++)
    {
        for (auto &r : robots)
        {
            r.p.first += r.v.first, r.p.second += r.v.second;
            while (r.p.first < 0)
                r.p.first += width;
            while (r.p.first >= width)
                r.p.first -= width;
            while (r.p.second < 0)
                r.p.second += height;
            while (r.p.second >= height)
                r.p.second -= height;
        }

        //dump();
    }

    auto retval = 0, q1 = 0, q2 = 0, q3 = 0, q4 = 0;

    for (auto &r : robots)
    {
        if ((r.p.first < width / 2) && (r.p.second < height / 2))
            q1++;
        else if ((r.p.first < width / 2) && (r.p.second > height / 2))
            q2++;
        else if ((r.p.first > width / 2) && (r.p.second < height / 2))
            q3++;
        else if ((r.p.first > width / 2) && (r.p.second > height / 2))
            q4++;
    }

    return q1 * q2 * q3 * q4;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
        ;

    return -1;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 231221760);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
