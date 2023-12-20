#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

using Polygon = std::vector<std::pair<long long, long long>>;

std::string rdlu = "RDLU";
std::vector<int> dir_x{ 1, 0 , -1, 0 }, dir_y{ 0, 1, 0,-1 };

auto get_polygon_area(const Polygon &vertices) // shoelace formula
{
    auto area = 0LL;
    auto prev_x = vertices.back().first, prev_y = vertices.back().second;

    for (auto &v : vertices)
    {
        auto x = v.first, y = v.second;
        area += (prev_x + x) * (prev_y - y);
        prev_x = x, prev_y = y;
    }

    return abs(area / 2);
}

auto solve(const std::string &filename, bool part2)
{
    Polygon vertices;
    std::ifstream file(filename);
    auto dir = ' ';
    auto len = 0, boundary_count = 0, dir_index = 0;
    std::string color;
    auto x = 0LL, y = 0LL;

    while (file >> dir >> len >> color)
    {
        if (part2)
            len = strtol(color.substr(2, 5).c_str(), NULL, 16);

        dir_index = part2 ? (color[7] - '0') : int(rdlu.find(dir));
        x += len * dir_x[dir_index];
        y += len * dir_y[dir_index];

        boundary_count += len;
        vertices.push_back({ x, y });
    }

    auto area = get_polygon_area(vertices);
    auto interior = area - boundary_count / 2 + 1; // Pick's theorem
    return interior + boundary_count;
}

int main()
{
    auto part1 = solve("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = solve("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 48652);
    assert(part2 == 45757884535661);
    return 0;
}
