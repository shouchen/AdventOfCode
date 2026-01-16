#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <cassert>

using Coordinates = std::array<int, 2>;
using CoordinatesList = std::vector<Coordinates>;
using Map = std::vector<std::vector<int>>;

CoordinatesList read_data(const std::string &filename)
{
    std::ifstream file(filename);
    CoordinatesList retval;
    auto row = 0, col = 0;
    auto comma = ',';

    while (file >> row >> comma >> col)
        retval.push_back({ row, col });

    return retval;
}

auto compress_cooordinates(const CoordinatesList &points)
{
    std::set<int> rows, cols;
    for (const auto &p : points)
        rows.insert(p[0]), cols.insert(p[1]);

    CoordinatesList compressed_points;
    compressed_points.reserve(points.size());

    for (const auto &p : points)
        compressed_points.push_back(
            Coordinates {
                int(std::distance(std::begin(rows), rows.find(p[0]))),
                int(std::distance(std::begin(cols), cols.find(p[1])))
            }
        );

    return compressed_points;
}

auto create_map(const CoordinatesList &points) {
    const auto col_max = (*std::max_element(std::begin(points), std::end(points), [](const auto &p1, const auto &p2) {return p1[1] < p2[1]; }))[1];
    const auto row_max = (*std::max_element(std::begin(points), std::end(points), [](const auto &p1, const auto &p2) {return p1[0] < p2[0]; }))[0];
    Map map(row_max + 1, std::vector<int>(col_max + 1, 0));

    for (auto i = 1; i < points.size(); i++)
    {
        const auto d_r = points[i][0] - points[i - 1][0], d_c = points[i][1] - points[i - 1][1];
        const auto delta = Coordinates{ d_r ? d_r / std::abs(d_r) : 0, d_c ? d_c / std::abs(d_c) : 0, };

        auto current = points[i - 1];
        while (current != points[i])
        {
            map[current[0]][current[1]] = 1;
            current[0] += delta[0];
            current[1] += delta[1];
        }
    }

    return map;
}

auto flood_fill(Map &map)
{
    Coordinates p{ 0, 0 };

    auto index = 0;
    while (map[index][0] == 0)
        index++;

    p = { index + 1, 1 };

    std::queue<Coordinates> q;
    q.push(p);

    const CoordinatesList adj_delta{ {1,0}, {0,-1}, {0,1}, {-1,0} };

    while (!q.empty())
    {
        const auto c = q.front();
        q.pop();

        map[c[0]][c[1]] = 2;

        for (const auto &d : adj_delta)
        {
            const auto n = Coordinates{ c[0] + d[0], c[1] + d[1] };

            if (n[0] >= 0 && n[0] < map.size() && n[1] >= 0 && n[1] < map[0].size() &&
                map[n[0]][n[1]] == 0)
            {
                q.push(n);
                map[n[0]][n[1]] = -1;
            }
        }
    }
}

auto is_valid_rectangle(const Map &map, const Coordinates &p1, const Coordinates &p2)
{
    for (auto row = std::min(p1[0], p2[0]); row <= std::max(p1[0], p2[0]); row++)
        for (auto col = std::min(p1[1], p2[1]); col <= std::max(p1[1], p2[1]); col++)
            if (map[row][col] == 0) return false;

    return true;
}

auto find_largest_rectangle(const Map &map, const CoordinatesList &compressed_points, const CoordinatesList &points)
{
    auto max_area = 0ULL;

    for (auto i = 0; i < points.size(); i++)
        for (auto j = 0; j < points.size(); j++)
            if (is_valid_rectangle(map, compressed_points[i], compressed_points[j]))
            {
                unsigned long long area = std::abs((points[i][0] - points[j][0]) + 1) * (std::abs(points[i][1] - points[j][1]) + 1);
                max_area = std::max(max_area,area);
            }

    return max_area;
}

auto do_part1(const std::string &filename)
{
    auto points = read_data(filename);
    auto max_area = 0LL;

    for (auto i = 0; i < points.size(); i++)
        for (auto j = 0; j < points.size(); j++)
            if (i != j)
            {
                auto area =
                    std::abs(long long(points[i][0] - points[j][0] + 1)) *
                    std::abs(long long(points[i][1] - points[j][1] + 1));
                max_area = std::max(max_area, area);
            }

    return max_area;
}

auto do_part2(const std::string &filename)
{
    auto points = read_data(filename);
    points.push_back(points[0]);

    const auto compressed_points = compress_cooordinates(points);
    auto map = create_map(compressed_points);
    flood_fill(map);

    return find_largest_rectangle(map, compressed_points, points);
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 4782896435);
                                                 
    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 1540060480);

    return 0;
}
