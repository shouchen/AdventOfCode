#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cassert>

struct Cube
{
    int x, y, z;
};

bool operator<(const Cube &lhs, const Cube &rhs)
{
    if (lhs.x < rhs.x) return true;
    if (lhs.x > rhs.x) return false;
    if (lhs.y < rhs.y) return true;
    if (lhs.y > rhs.y) return false;
    return lhs.z < rhs.z;
}

std::vector<Cube> cubes;
std::set<Cube> occupied, exterior;

auto min_x = INT_MAX, min_y = INT_MAX, min_z = INT_MAX;
auto max_x = INT_MIN, max_y = INT_MIN, max_z = INT_MIN;

auto find_empty_outside_cube()
{
    for (auto x = min_x; x <= max_x; x++)
        for (auto y = min_y; y <= max_y; y++)
            for (auto z = min_z; z <= max_z; z++)
            {
                Cube cube{ x,y,z };
                if (occupied.find(cube) == occupied.end())
                    return cube;
            }

    assert(false);
    return Cube{ 0,0,0 };
}

void flood_fill_exteriors(const Cube &cube)
{
    if (cube.x < min_x - 1 || cube.y < min_y - 1 || cube.z < min_z - 1 ||
        cube.x > max_x + 1 || cube.y > max_y + 1 || cube.z > max_z + 1)
        return;

    if (occupied.find(cube) != occupied.end() || exterior.find(cube) != exterior.end())
        return;

    exterior.insert(cube);
    flood_fill_exteriors(Cube{ cube.x - 1, cube.y, cube.z });
    flood_fill_exteriors(Cube{ cube.x + 1, cube.y, cube.z });
    flood_fill_exteriors(Cube{ cube.x, cube.y - 1, cube.z });
    flood_fill_exteriors(Cube{ cube.x, cube.y + 1, cube.z });
    flood_fill_exteriors(Cube{ cube.x, cube.y, cube.z - 1 });
    flood_fill_exteriors(Cube{ cube.x, cube.y, cube.z + 1 });
}

auto process_input(const std::string &filename)
{
    std::ifstream file(filename);
    auto x = 0, y = 0, z = 0;
    auto comma = ',';

    while (file >> x >> comma >> y >> comma >> z)
    {
        Cube cube{ x, y, z };
        cubes.push_back(cube);
        occupied.insert(cube);

        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);
        min_z = std::min(min_z, z);
        max_z = std::max(max_z, z);
    }

    Cube cube = find_empty_outside_cube();
    flood_fill_exteriors(cube);

    auto part1 = 0;
    for (const Cube &cube : cubes)
    {
        if (occupied.find(Cube{ cube.x - 1, cube.y, cube.z }) == occupied.end()) part1++;
        if (occupied.find(Cube{ cube.x + 1, cube.y, cube.z }) == occupied.end()) part1++;
        if (occupied.find(Cube{ cube.x, cube.y - 1, cube.z }) == occupied.end()) part1++;
        if (occupied.find(Cube{ cube.x, cube.y + 1, cube.z }) == occupied.end()) part1++;
        if (occupied.find(Cube{ cube.x, cube.y, cube.z - 1 }) == occupied.end()) part1++;
        if (occupied.find(Cube{ cube.x, cube.y, cube.z + 1 }) == occupied.end()) part1++;
    }

    auto part2 = 0;
    for (const Cube &cube : cubes)
    {
        if (exterior.find(Cube{ cube.x - 1, cube.y, cube.z }) != exterior.end()) part2++;
        if (exterior.find(Cube{ cube.x + 1, cube.y, cube.z }) != exterior.end()) part2++;
        if (exterior.find(Cube{ cube.x, cube.y - 1, cube.z }) != exterior.end()) part2++;
        if (exterior.find(Cube{ cube.x, cube.y + 1, cube.z }) != exterior.end()) part2++;
        if (exterior.find(Cube{ cube.x, cube.y, cube.z - 1 }) != exterior.end()) part2++;
        if (exterior.find(Cube{ cube.x, cube.y, cube.z + 1 }) != exterior.end()) part2++;
    }

    return std::make_pair(part1, part2);
}

int main()
{
    auto output = process_input("input.txt");
    std::cout << "Part One: " << output.first << std::endl;
    std::cout << "Part Two: " << output.second << std::endl;

    assert(output.first == 4636);
    assert(output.second == 2572);
    return 0;
}
