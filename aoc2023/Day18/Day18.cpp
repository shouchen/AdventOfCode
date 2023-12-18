#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

std::vector<std::pair<long long, long long>> vertices;

auto get_polygon_area() // shoelace formula
{
    auto n = vertices.size();
    auto area = 0LL;

    auto j = n - 1;
    for (auto i = 0; i < n; i++)
    {
        area += (vertices[j].first + vertices[i].first) * (vertices[j].second - vertices[i].second);
        j = i;
    }

    return abs(area / 2);
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    auto dir = ' ';
    auto len = 0;
    std::string colorstr;

    auto x = 0LL, y = 0LL;
    auto boundary_count = 0;

    vertices.clear();
    while (file >> dir >> len >> colorstr)
    {
        boundary_count += len;

        if (dir == 'R') x += len;
        else if (dir == 'D') y += len;
        else if (dir == 'L') x -= len;
        else if (dir == 'U') y -= len;

        vertices.push_back({ x, y });
    }

    auto area = get_polygon_area();
    auto interior = area - boundary_count / 2 + 1; // Pick's theorem
    return interior + boundary_count;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    auto dir = ' ';
    auto len = 0;
    std::string colorstr;

    auto x = 0LL, y = 0LL;
    auto boundary_count = 0;

    vertices.clear();
    while (file >> dir >> len >> colorstr)
    {
        len = strtol(colorstr.substr(2, 5).c_str(), NULL, 16);
        boundary_count += len;

        if (colorstr[7] == '0') x += len;
        else if (colorstr[7] == '1') y += len;
        else if (colorstr[7] == '2') x -= len;
        else if (colorstr[7] == '3') y -= len;

        vertices.push_back({ x, y });
    }

    auto area = get_polygon_area();
    auto interior = area - boundary_count / 2 + 1; // Pick's theorem
    return interior + boundary_count;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 48652);
    assert(part2 == 45757884535661);
    return 0;
}
