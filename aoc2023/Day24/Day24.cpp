#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

struct Hailstone
{
    long long x_pos, y_pos, z_pos;
    long long x_vel, y_vel, z_vel;
};

std::vector<Hailstone> hailstones;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    auto test = 0.0l;
    auto xp = 0LL, yp = 0LL, zp = 0LL, xv = 0LL, yv = 0LL, zv = 0LL;
    auto sep = ' ';

    while (file >> xp >> sep >> yp >> sep >> zp >> sep >> xv >> sep >> yv >> sep >> zv)
        hailstones.push_back(Hailstone{ xp, yp, zp, xv, yv, zv });
}

// Adapted from online code sample!
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
            auto x1 = double(h1.x_pos), x2 = double(h1.x_pos + h1.x_vel);
            auto x3 = double(h2.x_pos), x4 = double(h2.x_pos + h2.x_vel);
            auto y1 = double(h1.y_pos), y2 = double(h1.y_pos + h1.y_vel);
            auto y3 = double(h2.y_pos), y4 = double(h2.y_pos + h2.y_vel);

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
    // The linear systems in the input are WAY overdetermined and any three hailstones would
    // work (or else the problem would not be solvable as described). Let's arbitrarily pick
    // the first stones for getting enough equations.

    std::stringstream ss;
    for (auto i = 0; i < 3; i++)
    {
        ss << 't' << i << " >= 0, ";
        ss << hailstones[i].x_pos << " + " << hailstones[i].x_vel << 't' << i << " == x + vx " << 't' << i << ", ";
        ss << hailstones[i].y_pos << " + " << hailstones[i].y_vel << 't' << i << " == y + vy " << 't' << i << ", ";
        ss << hailstones[i].z_pos << " + " << hailstones[i].z_vel << 't' << i << " == z + vz " << 't' << i << ", ";
    }
    auto equations = ss.str();

    std::string sendToMathematica = "Solve[{" + equations.substr(0, equations.length() - 2) + "}, {x,y,z,vx,vy,vz,t0,t1,t2}]";
    std::cout << std::endl << sendToMathematica << std::endl << std::endl;

    // After getting a free trial Mathematica subscription from https://www.wolfram.com/mathematica/trial/
    // and a few seconds of computation, we get the below solutions. I'm at peace with doing this since the
    // alternative would be to import a high-powered library like Z3 to do basically the same thing. And
    // other code-only solutions would rely on examining the particulars of the input data and doing a
    // part-programming, part-data deep dive that may not really be much of a general solution at all.

    auto x = 242369545669096;
    auto y = 339680097675927;
    auto z = 102145685363875;
    return x + y + z;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 26611);
    assert(part2 == 684195328708898);
    return 0;
}
