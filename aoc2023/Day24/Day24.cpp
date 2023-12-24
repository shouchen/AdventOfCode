#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

struct Hailstone
{
    double x_pos, y_pos, z_pos;
    double x_vel, y_vel, z_vel;
};

std::vector<Hailstone> hailstones;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    double xp, yp, zp, xv, yv, zv;
    auto sep = ' ';

    while (file >> xp >> sep >> yp >> sep >> zp >> sep >> xv >> sep >> yv >> sep >> zv)
        hailstones.push_back(Hailstone{ xp, yp, zp, xv, yv, zv });
}

// adapted from online code sample!
auto intersection(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    auto denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
    if (denom == 0) return std::make_pair(DBL_MAX, DBL_MAX);

    auto ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
    auto ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;
    return std::make_pair(x1 + ua * (x2 - x1), y1 + ua * (y2 - y1));
}

auto do_part1()
{
    const auto zone_low = 200000000000000, zone_high = 400000000000000;
    auto retval = 0;

    for (auto i = 0; i < hailstones.size(); i++)
        for (auto j = i + 1; j < hailstones.size(); j++)
        {
            Hailstone &h1 = hailstones[i], &h2 = hailstones[j];
            auto x1 = h1.x_pos, x2 = h1.x_pos + h1.x_vel; auto x3 = h2.x_pos, x4 = h2.x_pos + h2.x_vel; 
            auto y1 = h1.y_pos, y2 = h1.y_pos + h1.y_vel; auto y3 = h2.y_pos, y4 = h2.y_pos + h2.y_vel;

            auto res = intersection(x1, y1, x2, y2, x3, y3, x4, y4);
            auto x = res.first, y = res.second;

            if (((x > x1) == (x2 - x1 > 0)) && ((y > y1) == (y2 - y1 > 0)) &&
                ((x > x3) == (x4 - x3 > 0)) && ((y > y3) == (y4 - y3 > 0)) &&
                x >= zone_low && x <= zone_high && y >= zone_low && y <= zone_high)
                retval++;
        }

    return retval;
}

auto do_part2()
{
    return -1;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 26611);
    //assert(part2 == );
    return 0;
}
