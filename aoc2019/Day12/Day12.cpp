#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

struct Moon
{
    std::vector<int> pos, vel;

    Moon(int x, int y, int z) : pos{ x, y, z }, vel{ 0, 0, 0 } {};
    void Move() { for (auto i = 0; i < 3; i++) pos[i] += vel[i]; }
    auto PotentialEnergy() { return std::abs(pos[0]) + std::abs(pos[1]) + std::abs(pos[2]); }
    auto KineticEnergy() { return std::abs(vel[0]) + std::abs(vel[1]) + std::abs(vel[2]); }
};

struct Universe
{
    std::vector<Moon> moons;

    void AdjustGravities()
    {
        for (auto &moon1 : moons)
            for (auto &moon2 : moons)
                for (auto k = 0; k < 3; k++)
                    moon1.vel[k] += Compare(moon2.pos[k], moon1.pos[k]);
    }

    void Move()
    {
        for (auto &moon : moons)
            moon.Move();
    }

    void DoStep()
    {
        AdjustGravities();
        Move();
    }

    auto TotalEnergy()
    {
        auto total_energy = 0;
        for (auto &moon : moons)
            total_energy += moon.PotentialEnergy() * moon.KineticEnergy();
        return total_energy;
    }

    int Compare(int a, int b)
    {
        return (a < b) ? -1 : ((a > b) ? 1 : 0);
    }
};

Universe read_input(const std::string &filename)
{
    std::ifstream file("input.txt");
    auto comma = ',', open_angle_bracket = '<', close_angle_bracket = '>', v = ' ', equals = '=';
    auto x = 0, y = 0, z = 0;
    Universe u;

    while (file >> open_angle_bracket >> v >> equals >> x >> comma >> v >> equals >> y
                >> comma >> v >> equals >> z >> close_angle_bracket)
        u.moons.push_back(Moon(x, y, z));

    return u;
}

long long find_gcd(long long a, long long b)
{
    return b ? find_gcd(b, a % b) : a;
}

long long find_lcm(long long a, long long b)
{
    return a * b / find_gcd(a, b);
}

auto do_part1()
{
    Universe u = read_input("input.txt");

    for (auto i = 0; i < 1000; i++)
        u.DoStep();

    return u.TotalEnergy();
}

long long do_part2()
{
    Universe u = read_input("input.txt");
    auto inital = u;
    int repeat[3] { 0, 0, 0 };

    for (auto i = 1; !repeat[0] || !repeat[1] || !repeat[2]; i++)
    {
        u.DoStep();

        for (int j = 0; j < 3; j++)
        {
            if (!repeat[j])
            {
                auto equal = true;
                for (auto k = 0; k < 4; k++)
                {
                    equal = true;
                    if (u.moons[k].pos[j] != inital.moons[k].pos[j] ||
                        u.moons[k].vel[j] != inital.moons[k].vel[j])
                    {
                        equal = false;
                        break;
                    }
                }
                if (equal)
                    repeat[j] = i;
            }
        }
    }

    return find_lcm(repeat[0], find_lcm(repeat[1], repeat[2]));
}

int main()
{
    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 7722);
    assert(part2 == 292653556339368);
    return 0;
}
