#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>
#include <set>

struct Coords
{
    long long x, y, z;
 
    unsigned long long distance_from_origin() const { return abs(x) + abs(y) + abs(z); }

    Coords &operator+=(const Coords &rhs)
    {
        this->x += rhs.x; this->y += rhs.y; this->z += rhs.z;
        return *this;
    }
};

std::istream &operator>>(std::istream &i, Coords &coords)
{
    auto open_bracket = '<', close_bracket = '>', comma = ',';
    return i >> open_bracket >> coords.x >> comma >> coords.y >> comma >> coords.z >> close_bracket;
}

bool operator<(const Coords &lhs, const Coords &rhs)
{
    if (lhs.x < rhs.x) return true;
    if (lhs.x > rhs.x) return false;
    if (lhs.y < rhs.y) return true;
    if (lhs.y > rhs.y) return false;
    if (lhs.z < rhs.z) return true;
    if (lhs.z > rhs.z) return false;
    return false;
}

struct Particle
{
    unsigned index;
    bool exists = true;
    Coords p, v, a;

    void tick() { v += a; p += v; }
};

std::istream &operator>>(std::istream &i, Particle &part)
{
    auto p = 'p', v = 'v', a = 'a', equals = '=', comma = ',';
    return i >> p >> equals >> part.p >> comma >> v >> equals >> part.v >> comma >> a >> equals >> part.a;
}

std::vector<Particle> particles;

bool mydistfunc(const Particle &a, const Particle &b)
{
    auto a_dist = a.p.distance_from_origin(), b_dist = b.p.distance_from_origin();
    if (a_dist < b_dist) return true;
    if (b_dist < a_dist) return false;
    return a.index < b.index;
}

bool myfunction(const Particle &a, const Particle &b)
{
    auto acca = abs(a.a.x) + abs(a.a.y) + abs(a.a.z);
    auto accb = abs(b.a.x) + abs(b.a.y) + abs(b.a.z);
    auto vela = abs(a.v.x) + abs(a.v.y) + abs(a.v.z);
    auto velb = abs(b.v.x) + abs(b.v.y) + abs(b.v.z);
    auto posa = abs(a.p.x) + abs(a.p.y) + abs(a.p.z);
    auto posb = abs(b.p.x) + abs(b.p.y) + abs(b.p.z);
    //auto acca2 = distance_from_origin(a.a);
    //auto accb2 = distance_from_origin(b.a);
    //auto vela2 = distance_from_origin(a.v);
    //auto velb2 = distance_from_origin(b.v);
    //auto posa2 = distance_from_origin(a.p);
    //auto posb2 = distance_from_origin(b.p);

    //assert(acca == acca2);
    //assert(accb == accb2);
    //assert(vela == vela2);
    //assert(velb == velb2);
    //assert(posa == posa2);
    //assert(posb == posb2);

    if (acca < accb) return true;
    if (accb < acca) return false;

    if (vela < velb) return true;
    if (velb < vela) return false;

    if (posa < posb) return true;
    if (posb < posa) return false;

    if (a.index < b.index) return true;
    if (b.index < a.index) return false;

    return false;
    assert(false);
}

bool mysortfunc(int a, int b)
{
    return b < a;
}

long long dot_product(Particle &p)
{
    return
        p.a.x * p.v.x +
        p.a.y * p.v.y +
        p.a.z * p.v.z;
}

void read_data(const std::string &filename)
{
    std::ifstream f("input.txt");
    particles.clear();
    Particle part;
    unsigned index = 0;

    while (f >> part)
    {
        part.index = index++;
        particles.push_back(part);
    }
}

// Note: also alters order
unsigned do_part1(const std::string &filename)
{
    read_data(filename);

    for (auto times = 0; times < 1000; times++) // TODO: Need better end condition! (really is 344x)
    {
        std::sort(particles.begin(), particles.end(), mydistfunc);
        for (auto &i : particles)
            i.tick();
    }

    return particles[0].index;
}

unsigned do_part2(const std::string &filename)
{
    read_data(filename);

    std::map<Coords, unsigned> first_with_coords;

    for (auto times = 0; times < 100; times++) // TODO: Need better end condition!
    {
        first_with_coords.clear();

        for (auto &i : particles)
        {
            if (!i.exists)
                continue;

            i.tick();

            auto found = first_with_coords.find(i.p);
            if (found != first_with_coords.end())
            {
                particles[found->second].exists = false;
                i.exists = false;
            }
            else
                first_with_coords[i.p] = i.index;
        }
    }

    unsigned ret_val = 0;
    for (auto i : particles)
        if (i.exists)
            ret_val++;

    return ret_val;
}

int main()
{
    auto part1 = do_part1("input.txt");
    auto part2 = do_part2("input.txt");

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 308);
    assert(part2 == 504);
    return 0;
}
