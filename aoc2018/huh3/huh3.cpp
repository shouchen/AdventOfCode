#include "stdafx.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <vector>

struct Nanobot
{
    int64_t x, y, z, r;
};

std::ostream &operator<<(std::ostream &os, const Nanobot &nanobot)
{
    os << "pos=<" << nanobot.x << "," << nanobot.y << "," << nanobot.z
        << ">, r=" << nanobot.r;
    return os;
}

//std::istream &operator>>(std::istream &is, Nanobot &nanobot)
//{
//    int x, y, z, radius;
//    char c, comma;
//
//    while (is >> c >> c >> c >> c >> c >> x >> comma >> y >> comma >> z >> c >> comma >> c >> c >> radius)
//    {
//        nanobots.push_back(Nanobot{ x, y, z, radius });
//        if (nanobots.back().radius > nanobots[strongest].radius)
//            strongest = nanobots.size() - 1;
//        min_x = std::min(min_x, x);
//        max_x = std::max(max_x, x);
//        min_y = std::min(min_y, y);
//        max_y = std::max(max_y, y);
//        min_z = std::min(min_z, z);
//        max_z = std::max(max_z, z);
//    }
//
//
//
//    std::string line;
//    std::getline(is, line);
//    if (is)
//    {
//        std::vector<std::string> elements;
//        boost::split(elements, line, boost::is_any_of("<,>="));
//        nanobot.x = std::stoi(elements.at(2));
//        nanobot.y = std::stoi(elements.at(3));
//        nanobot.z = std::stoi(elements.at(4));
//        nanobot.r = std::stoi(elements.at(7));
//    }
//    return is;
//}

bool covers(const Nanobot &nanobot, const int64_t &x, const int64_t &y,
    const int64_t &z, const int64_t &padding)
{
    return (std::abs(x - nanobot.x) + std::abs(y - nanobot.y)
        + std::abs(z - nanobot.z)
        <= nanobot.r + padding);
}

struct Point
{
    int64_t x, y, z;
    Point(const int64_t &X, const int64_t &Y, const int64_t &Z)
        : x(X), y(Y), z(Z)
    {}
    bool operator<(const Point &p) const
    {
        return x < p.x
            ? true
            : (x > p.x ? false
                : (y < p.y ? true : (y > p.y ? false : z < p.z)));
    }

    bool operator==(const Point &p) const
    {
        return x == p.x && y == p.y && z == p.z;
    }
};

std::ostream &operator<<(std::ostream &os, const Point &point)
{
    os << "(" << point.x << "," << point.y << "," << point.z << ")";
    return os;
}

bool covers(const Nanobot &nanobot, const Point &p, const int64_t &padding)
{
    return covers(nanobot, p.x, p.y, p.z, padding);
}

int main(int, char *argv[])
{
    std::ifstream file("input.txt");
    int x, y, z, radius;
    char c, comma;

    std::vector<Nanobot> nanobots;
    while (file >> c >> c >> c >> c >> c >> x >> comma >> y >> comma >> z >> c >> comma >> c >> c >> radius)
    {
        nanobots.push_back(Nanobot{ x, y, z, radius });
    }

    std::sort(nanobots.begin(), nanobots.end(),
        [](const Nanobot &n0, const Nanobot &n1) { return n0.r < n1.r; });

    auto &nanobot(nanobots.back());

    std::cout << "Part 1: "
        << std::count_if(nanobots.begin(), nanobots.end(),
            [&](const Nanobot &n) {
        return covers(nanobot, n.x, n.y, n.z, 0);
    })
        << "\n";

    int64_t x_min(0), y_min(0), z_min(0), x_max(0), y_max(0), z_max(0);
    for (auto &n : nanobots)
    {
        x_min = std::min(x_min, n.x - n.r);
        x_max = std::max(x_max, n.x + n.r + 1);
        y_min = std::min(y_min, n.y - n.r);
        y_max = std::max(y_max, n.y + n.r + 1);
        z_min = std::min(z_min, n.z - n.r);
        z_max = std::max(z_max, n.z + n.r + 1);
    }

    int64_t scale(
        int64_t(1) << int64_t(
            std::log2(x_max - x_min + y_max - y_min + z_max - z_min) + 1));
    x_min = (x_min / scale) * scale;
    x_max = (x_max / scale + 1) * scale;
    y_min = (y_min / scale) * scale;
    y_max = (y_max / scale + 1) * scale;
    z_min = (z_min / scale) * scale;
    z_max = (z_max / scale + 1) * scale;

    size_t nx = (x_max - x_min) / scale;
    size_t ny = (y_max - y_min) / scale;
    size_t nz = (z_max - z_min) / scale;

    std::vector<Point> points;
    for (size_t dx = 0; dx < nx; ++dx)
        for (size_t dy = 0; dy < ny; ++dy)
            for (size_t dz = 0; dz < nz; ++dz)
            {
                points.emplace_back(x_min + dx * scale, y_min + dy * scale,
                    z_min + dz * scale);
            }

    while (true)
    {
        size_t max_bots(0);
        std::vector<Point> new_points;
        for (auto &point : points)
        {
            size_t num_bots(std::count_if(
                nanobots.begin(), nanobots.end(),
                [&](const Nanobot &n) { return covers(n, point, scale); }));

            if (num_bots != 0 && num_bots == max_bots)
            {
                new_points.emplace_back(point);
            }
            if (num_bots > max_bots)
            {
                max_bots = num_bots;
                new_points.clear();
                new_points.emplace_back(point);
            }
        }

        if (scale == 0)
        {
            std::swap(points, new_points);
            break;
        }
        points.clear();
        scale /= 2;
        if (scale == 0)
        {
            std::swap(points, new_points);
        }
        else
        {
            for (auto &point : new_points)
            {
                for (int64_t dx = -scale; dx <= scale; dx += scale)
                    for (int64_t dy = -scale; dy <= scale; dy += scale)
                        for (int64_t dz = -scale; dz <= scale; dz += scale)
                            points.emplace_back(point.x + dx, point.y + dy,
                                point.z + dz);
            }
            std::sort(points.begin(), points.end());
            auto last(std::unique(points.begin(), points.end()));
            points.erase(last, points.end());
        }
    }

    int64_t min_distance(std::numeric_limits<int64_t>::max());
    for (auto &point : points)
    {
        min_distance
            = std::min(min_distance,
                std::abs(point.x) + std::abs(point.y) + std::abs(point.z));
    }

    std::cout << "Part 2: " << min_distance << "\n";
}
//110841112
