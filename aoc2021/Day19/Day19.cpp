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

struct MyCoords
{
    int x, y, z;
    bool operator<(const MyCoords &rhs) const
    {
        if (x < rhs.x) return true; else if (x > rhs.x) return false;
        if (y < rhs.y) return true; else if (y > rhs.y) return false;
        return z < rhs.z;
    }
};

struct Scanner
{
    std::vector<std::vector<MyCoords>> beacons; // first index is orientation (0-24), second is list of points
    MyCoords *coords = nullptr; // location of the beacon (if known, else nullptr)
};

std::vector<Scanner> my_scanners;
std::set<MyCoords> all_beacons; // relative to scanner 0

std::vector<MyCoords> my_get_orientation(const std::vector<MyCoords> &ref, int orientation)
{
    std::vector<MyCoords> retval;
    switch (orientation)
    {
    case  0: for (auto &b : ref) retval.push_back(MyCoords{ b.x,  b.y,  b.z}); break;
    case  1: for (auto &b : ref) retval.push_back(MyCoords{ b.x, -b.z,  b.y}); break;
    case  2: for (auto &b : ref) retval.push_back(MyCoords{ b.x, -b.y, -b.z}); break;
    case  3: for (auto &b : ref) retval.push_back(MyCoords{ b.x,  b.z, -b.y}); break;
    case  4: for (auto &b : ref) retval.push_back(MyCoords{ b.y,  b.z,  b.x}); break;
    case  5: for (auto &b : ref) retval.push_back(MyCoords{ b.y, -b.x,  b.z}); break;
    case  6: for (auto &b : ref) retval.push_back(MyCoords{ b.y, -b.z, -b.x}); break;
    case  7: for (auto &b : ref) retval.push_back(MyCoords{ b.y,  b.x, -b.z}); break;
    case  8: for (auto &b : ref) retval.push_back(MyCoords{ b.z,  b.x,  b.y}); break;
    case  9: for (auto &b : ref) retval.push_back(MyCoords{ b.z, -b.y,  b.x}); break;
    case 10: for (auto &b : ref) retval.push_back(MyCoords{ b.z, -b.x, -b.y}); break;
    case 11: for (auto &b : ref) retval.push_back(MyCoords{ b.z,  b.y, -b.x}); break;
    case 12: for (auto &b : ref) retval.push_back(MyCoords{-b.z, -b.y, -b.x}); break;
    case 13: for (auto &b : ref) retval.push_back(MyCoords{-b.z,  b.x, -b.y}); break;
    case 14: for (auto &b : ref) retval.push_back(MyCoords{-b.z,  b.y,  b.x}); break;
    case 15: for (auto &b : ref) retval.push_back(MyCoords{-b.z, -b.x,  b.y}); break;
    case 16: for (auto &b : ref) retval.push_back(MyCoords{-b.y, -b.x, -b.z}); break;
    case 17: for (auto &b : ref) retval.push_back(MyCoords{-b.y,  b.z, -b.x}); break;
    case 18: for (auto &b : ref) retval.push_back(MyCoords{-b.y,  b.x,  b.z}); break;
    case 19: for (auto &b : ref) retval.push_back(MyCoords{-b.y, -b.z,  b.x}); break;
    case 20: for (auto &b : ref) retval.push_back(MyCoords{-b.x, -b.z, -b.y}); break;
    case 21: for (auto &b : ref) retval.push_back(MyCoords{-b.x,  b.y, -b.z}); break;
    case 22: for (auto &b : ref) retval.push_back(MyCoords{-b.x,  b.z,  b.y}); break;
    case 23: for (auto &b : ref) retval.push_back(MyCoords{-b.x, -b.y,  b.z}); break;
    }
    return retval;
};

void read_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string scanner_header, s;
    auto x = 0, y = 0, z = 0;
    auto comma = ',';

    while (std::getline(file, scanner_header))
    {
        Scanner my_scanner;
        std::vector<MyCoords> coords;

        while (std::getline(file, s) && s.length())
        {
            std::stringstream ss(s);
            ss >> x >> comma >> y >> comma >> z;
            coords.push_back(MyCoords{ x,y,z });
        }
        my_scanner.beacons.push_back(coords);

        for (auto i = 1; i < 24; i++)
        {
            auto reoriented = my_get_orientation(my_scanner.beacons[0], i);
            my_scanner.beacons.push_back(reoriented);
        }

        my_scanners.push_back(my_scanner);
    }
}

auto eliminate_one_scanner()
{
    for (auto &s : my_scanners)
    {
        if (s.coords)
            continue; // already eliminated

        // loop over each orientation of this scanner
        for (int i = 0; i < 24; i++)
        {
            std::map<std::tuple<int, int, int>, int> deltas_count;

            // try all possible pairs of "all" and this orientation, computing dx, dy, dz
            for (auto &a : all_beacons)
                for (auto &b : s.beacons[i])
                {
                    auto d = std::make_tuple(b.x - a.x, b.y - a.y, b.z - a.z);
                    deltas_count[d]++;
                }

            // if more than 12 had the same dx, dy, dz it's an overlap
            for (auto &dc : deltas_count)
            {
                if (dc.second >= 12)
                {
                    std::cout << "HIT" << std::endl;

                    // Add beacons from this one to "all" (with delta adjustments)
                    for (auto &b : s.beacons[i])
                        all_beacons.insert(MyCoords{b.x - std::get<0>(dc.first), b.y - std::get<1>(dc.first), b.z - std::get<2>(dc.first) });

                    // Add the location of this scanner (for part and so it will be skipped in part1)
                    s.coords = new MyCoords{std::get<0>(dc.first), std::get<1>(dc.first), std::get<2>(dc.first) }; // TODO: Real location?

                    return true;
                }
            }
        }
    }

    return false;
}

auto solve()
{
    for (auto &c : my_scanners[0].beacons[0])
        all_beacons.insert(c);
    my_scanners[0].coords = new MyCoords{ 0, 0, 0 };

    while (eliminate_one_scanner());

    auto part2 = 0;
    for (auto &s1 : my_scanners)
        for (auto &s2 : my_scanners)
        {
            auto md = std::abs(s1.coords->x - s2.coords->x) + std::abs(s1.coords->y - s2.coords->y) + std::abs(s1.coords->z - s2.coords->z);
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
