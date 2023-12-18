#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cassert>

std::map<std::pair<long long, long long>, int> grid;

auto min_x = LLONG_MAX, min_y = LLONG_MAX;
auto max_x = LLONG_MIN, max_y = LLONG_MIN;

void set_cell(long long x, long long y)
{
    min_x = std::min(min_x, x);
    min_y = std::min(min_y, y);
    max_x = std::max(max_x, x);
    max_y = std::max(max_y, y);
    grid[std::make_pair(x, y)] = 1;
}

void set_cell_dot(long long x, long long y)
{
    min_x = std::min(min_x, x);
    min_y = std::min(min_y, y);
    max_x = std::max(max_x, x);
    max_y = std::max(max_y, y);
    grid[std::make_pair(x, y)] = 2;  // 1 == #, 2 == .
}

auto count_interior()
{
    auto interior = 0LL;
    for (auto i = min_y; i <= max_y; i++)
        for (auto j = min_x; j <= max_x; j++)
            if (grid.find(std::make_pair(j, i)) == grid.end() || grid[std::make_pair(j, i)] == 1)
                interior++;

    return interior;
}

void recur(int x, int y)
{
    if (x < min_x || x > max_x || y < min_y || y > max_y)
        return;
        
    if (grid.find(std::make_pair(x, y)) != grid.end())
        return;

    set_cell_dot(x, y);
    recur(x - 1, y);
    recur(x + 1, y);
    recur(x, y - 1);
    recur(x, y + 1);
}

void fill_outside()
{
    for (auto x = min_x; x <= max_x; x++)
    {
        recur(x, min_y);
        recur(x, max_y);
    }

    for (auto y = min_y; y <= max_y; y++)
    {
        recur(min_x, y);
        recur(max_x, y);
    }
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    auto dir = ' ';
    auto len = 0;
    std::string color;

    auto x = 0, y = 0;
    auto xd = 0, yd = -1;

    grid[std::make_pair(x, y)] = 1;

    while (file >> dir >> len >> color)
    {
        if (dir == 'R') xd = 1, yd = 0;
        else if (dir == 'D') xd = 0, yd = 1;
        else if (dir == 'L') xd = -1, yd = 0;
        else if (dir == 'U') xd = 0, yd = -1;

        while (len--)
        {
            x += xd, y += yd;
            set_cell(x, y);
        }
    }

    fill_outside();
    return count_interior();
}

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

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    auto dir = ' ';
    auto len = 0;
    std::string colorstr;

    auto x = 0LL, y = 0LL;
    auto boundary_count = 0;

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
