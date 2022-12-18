#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cassert>

struct Cube
{
    int x, y, z;
};

auto operator<(const Cube &lhs, const Cube &rhs)
{
    if (lhs.x < rhs.x) return true; if (lhs.x > rhs.x) return false;
    if (lhs.y < rhs.y) return true; if (lhs.y > rhs.y) return false;
    return lhs.z < rhs.z;
}

std::vector<Cube> cubes;
std::set<Cube> occupied, exterior;

auto min_x = INT_MAX, min_y = INT_MAX, min_z = INT_MAX;
auto max_x = INT_MIN, max_y = INT_MIN, max_z = INT_MIN;

auto get_neighbors(const Cube &cube)
{
    return std::vector<Cube>{
        Cube{ cube.x - 1, cube.y, cube.z }, Cube{ cube.x + 1, cube.y, cube.z },
        Cube{ cube.x, cube.y - 1, cube.z }, Cube{ cube.x, cube.y + 1, cube.z },
        Cube{ cube.x, cube.y, cube.z - 1 }, Cube{ cube.x, cube.y, cube.z + 1 }
    };
}

void flood_fill_exteriors(const Cube &cube)
{
    if (cube.x >= min_x - 1 && cube.y >= min_y - 1 && cube.z >= min_z - 1 &&
        cube.x <= max_x + 1 && cube.y <= max_y + 1 && cube.z <= max_z + 1 &&
        occupied.find(cube) == occupied.end() &&
        exterior.find(cube) == exterior.end())
    {
        exterior.insert(cube);
        for (auto &c : get_neighbors(cube))
            flood_fill_exteriors(c);
    }
}

auto process_input(const std::string &filename)
{
    std::ifstream file(filename);
    auto x = 0, y = 0, z = 0;
    auto comma = ',';
    auto retval = std::make_pair(0, 0);

    while (file >> x >> comma >> y >> comma >> z)
    {
        Cube cube{ x, y, z };
        cubes.push_back(cube);
        occupied.insert(cube);

        min_x = std::min(min_x, x); max_x = std::max(max_x, x);
        min_y = std::min(min_y, y); max_y = std::max(max_y, y);
        min_z = std::min(min_z, z); max_z = std::max(max_z, z);
    }

    flood_fill_exteriors(Cube{ min_x - 1, min_y - 1, min_z - 1 });

    for (auto &c1 : cubes)
        for (auto &c2 : get_neighbors(c1))
        {
            if (occupied.find(c2) == occupied.end()) retval.first++;
            if (exterior.find(c2) != exterior.end()) retval.second++;
        }

    return retval;
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
