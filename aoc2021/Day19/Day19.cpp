#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <cmath>
#include <cassert>

struct Coords
{
    int x, y, z;
    bool operator<(const Coords &rhs) const
    {
        if (x < rhs.x) return true; else if (x > rhs.x) return false;
        if (y < rhs.y) return true; else if (y > rhs.y) return false;
        return z < rhs.z;
    }
};

struct Scanner
{
    std::vector<std::vector<Coords>> beacons;
    Coords *location = nullptr;
};

std::vector<Scanner> scanners;
std::set<Coords> all_beacons;

Coords reorient(Coords &c, int orientation)
{
    switch (orientation)
    {
    case  0: return Coords{  c.x,  c.y,  c.z };
    case  1: return Coords{  c.x, -c.z,  c.y };
    case  2: return Coords{  c.x, -c.y, -c.z };
    case  3: return Coords{  c.x,  c.z, -c.y };
    case  4: return Coords{  c.y,  c.z,  c.x };
    case  5: return Coords{  c.y, -c.x,  c.z };
    case  6: return Coords{  c.y, -c.z, -c.x };
    case  7: return Coords{  c.y,  c.x, -c.z };
    case  8: return Coords{  c.z,  c.x,  c.y };
    case  9: return Coords{  c.z, -c.y,  c.x };
    case 10: return Coords{  c.z, -c.x, -c.y };
    case 11: return Coords{  c.z,  c.y, -c.x };
    case 12: return Coords{ -c.z, -c.y, -c.x };
    case 13: return Coords{ -c.z,  c.x, -c.y };
    case 14: return Coords{ -c.z,  c.y,  c.x };
    case 15: return Coords{ -c.z, -c.x,  c.y };
    case 16: return Coords{ -c.y, -c.x, -c.z };
    case 17: return Coords{ -c.y,  c.z, -c.x };
    case 18: return Coords{ -c.y,  c.x,  c.z };
    case 19: return Coords{ -c.y, -c.z,  c.x };
    case 20: return Coords{ -c.x, -c.z, -c.y };
    case 21: return Coords{ -c.x,  c.y, -c.z };
    case 22: return Coords{ -c.x,  c.z,  c.y };
    case 23: return Coords{ -c.x, -c.y,  c.z };
    }
    return c;
};

void read_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string scanner_header, s;
    auto x = 0, y = 0, z = 0;
    auto comma = ',';

    while (std::getline(file, scanner_header))
    {
        Scanner scanner;
        for (auto i = 0; i < 24; i++)
            scanner.beacons.push_back(std::vector<Coords>());

        while (std::getline(file, s) && s.length())
        {
            std::stringstream ss(s);
            ss >> x >> comma >> y >> comma >> z;
            auto c = Coords{ x,y,z };

            for (auto i = 0; i < 24; i++)
                scanner.beacons[i].push_back(reorient(c, i));
        }

        scanners.push_back(scanner);
    }
}

auto eliminate_one_scanner()
{
    for (auto &scanner : scanners)
    {
        if (scanner.location)
            continue;

        for (int orientation = 0; orientation < 24; orientation++)
        {
            std::map<std::tuple<int, int, int>, int> deltas_count;

            for (auto &a : all_beacons)
                for (auto &b : scanner.beacons[orientation])
                {
                    auto dx = b.x - a.x, dy = b.y - a.y, dz = b.z - a.z;

                    if (++deltas_count[std::make_tuple(dx, dy, dz)] < 12)
                        continue;

                    for (auto &bb : scanner.beacons[orientation])
                        all_beacons.insert(Coords{ bb.x - dx, bb.y - dy, bb.z - dz });

                    scanner.location = new Coords{ dx, dy, dz };
                    return true;
                }
        }
    }

    return false;
}

auto solve()
{
    for (auto &c : scanners[0].beacons[0])
        all_beacons.insert(c);
    scanners[0].location = new Coords{ 0, 0, 0 };

    while (eliminate_one_scanner());

    auto part2 = 0;
    for (auto &s1 : scanners)
        for (auto &s2 : scanners)
        {
            auto md =
                std::abs(s1.location->x - s2.location->x) +
                std::abs(s1.location->y - s2.location->y) +
                std::abs(s1.location->z - s2.location->z);
            part2 = std::max(part2, md);
        }

    return std::make_pair(all_beacons.size(), part2);
}

int main()
{
    read_data("input.txt");
    auto result = solve();

    std::cout << "Part 1: " << result.first << std::endl;
    std::cout << "Part 2: " << result.second << std::endl;

    assert(result.first == 359);
    assert(result.second == 12292);
    return 0;
}
