#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>
#include <cassert>
#include "..\Utils\Utils.h"

set<tuple<int, int, int>> grid;

auto do_part1(const std::string &filename)
{
    std::ifstream file("input.txt");
    std::string player, n, starting, position, line, on_off;
    auto input = std::make_pair(0, 0);
    char c;

    int x1, x2, y1, y2, z1, z2;
    while (file >> on_off >> c >> c >> x1 >> c >> c >> x2 >> c >> c >> c >> y1 >> c >> c >> y2 >> c >> c >> c >> z1 >> c >> c >> z2)
    {
        if (x1 < -50) continue;
        if (y1 < -50) continue;
        if (z1 < -50) continue;
        if (x1 > 50) continue;
        if (y1 > 50) continue;
        if (z1 > 50) continue;

        for (int x = x1 ; x <= x2; x++)
            for (int y = y1; y <= y2; y++)
                for (int z = z1; z <= z2; z++)
                {
                    if (on_off == "on")
                        grid.insert(make_tuple(x, y, z));
                    else if (on_off == "off")
                        grid.erase(make_tuple(x, y, z));
                    else assert(false);
                }
    }

    return grid.size();
}

struct Cuboid
{
    pair<long long, long long> x, y, z;
};

struct InputStep
{
    bool on;
    int xmin, xmax, ymin, ymax, zmin, zmax;
};


long long on_count = 0;

vector<Cuboid> cuboids;

bool contains(Cuboid &a, Cuboid &b)
{
    return
        a.x.first <= b.x.first && a.x.second >= b.x.second &&
        a.y.first <= b.y.first && a.y.second >= b.y.second &&
        a.z.first <= b.z.first && a.z.second >= b.z.second;
}

bool intersects(Cuboid &a, Cuboid &b)
{
    return
        a.x.first <= b.x.second && a.x.second >= b.x.first &&
        a.y.first <= b.y.second && a.y.second >= b.y.first &&
        a.z.first <= b.z.second && a.z.second >= b.z.first;
}

// if second < first, not an intersection
pair<int, int> range_intersection(const pair<int, int> &r1, const pair<int, int> &r2)
{
    pair<int, int> retval = make_pair(std::max(r1.first, r2.first), std::min(r1.second, r2.second));
    return retval;
}

long long cuboid_intersection(const Cuboid &c1, const Cuboid &c2)
{
    auto ri_x = range_intersection(c1.x, c2.x);
    auto ri_y = range_intersection(c1.y, c2.y);
    auto ri_z = range_intersection(c1.z, c2.z);

    if (ri_x.first <= ri_x.second && ri_y.first <= ri_y.second && ri_z.first <= ri_z.second)
        return (ri_x.second - ri_x.first + 1) * (ri_y.second - ri_y.first + 1) * (ri_z.second - ri_z.first + 1);
    else
        return 0;
}

auto count()
{
    long long retval = 0;
    for (auto &c : cuboids)
        retval += (c.x.second - c.x.first + 1) * (c.y.second - c.y.first + 1) * (c.z.second - c.z.first + 1);

    return retval;
}

void merge_on_cuboid(Cuboid cuboid)
{
    if (cuboids.size() == 0)
    {
        on_count += (cuboid.x.second - cuboid.x.first + 1) * (cuboid.y.second - cuboid.y.first + 1) * (cuboid.z.second - cuboid.z.first + 1);
        cuboids.push_back(cuboid);
        cout << count() << endl;
    }
    else
    {
        for (auto &c : cuboids)
        {
            auto result = (cuboid_intersection(cuboid, c));
        }
    }
}

auto do_part2(const std::string &filename)
{
    std::vector<InputStep> steps;
    std::vector<int> x_seam, y_seam, z_seam;

    std::string s, on_off;
    char x = 'x', y = 'y', z = 'z', equals = '=', dot = '.', comma = ',';

    std::ifstream file(filename);

    InputStep step;
    while (file >> on_off >> x >> equals >> step.xmin >> dot >> dot >> step.xmax >> comma >>
        y >> equals >> step.ymin >> dot >> dot >> step.ymax >> comma >>
        z >> equals >> step.zmin >> dot >> dot >> step.zmax)
    {
        step.on = on_off == "on";
        steps.push_back(step);

        x_seam.push_back(step.xmin); x_seam.push_back(step.xmax + 1);
        y_seam.push_back(step.ymin); y_seam.push_back(step.ymax + 1);
        z_seam.push_back(step.zmin); z_seam.push_back(step.zmax + 1);
    }

    sort(x_seam.begin(), x_seam.end());
    sort(y_seam.begin(), y_seam.end());
    sort(z_seam.begin(), z_seam.end());

    std::vector<std::vector<std::vector<int>>> cubettes;
    cubettes.resize(x_seam.size(), std::vector<std::vector<int>>(y_seam.size(), std::vector<int>(z_seam.size())));

    //std::map<std::tuple<int, int, int>, bool> region;

    for (auto &step : steps)
    {
        auto x_min = lower_bound(x_seam.begin(), x_seam.end(), step.xmin) - x_seam.begin();
        auto x_max = lower_bound(x_seam.begin(), x_seam.end(), step.xmax + 1) - x_seam.begin();

        auto y_min = lower_bound(y_seam.begin(), y_seam.end(), step.ymin) - y_seam.begin();
        auto y_max = lower_bound(y_seam.begin(), y_seam.end(), step.ymax + 1) - y_seam.begin();

        auto z_min = lower_bound(z_seam.begin(), z_seam.end(), step.zmin) - z_seam.begin();
        auto z_max = lower_bound(z_seam.begin(), z_seam.end(), step.zmax + 1) - z_seam.begin();

        for (auto i = x_min; i < x_max; i++)
            for (auto j = y_min; j < y_max; j++)
                for (auto k = z_min; k < z_max; k++)
                {
                    cubettes[i][j][k] = step.on;
                    //region[make_tuple(i, j, k)] = step.on;
                }
    }

    auto retval = 0LL;
    for (auto i = 0; i < x_seam.size() - 1; i++)
        for (auto j = 0; j < y_seam.size() - 1; j++)
            for (auto k = 0; k < z_seam.size() - 1; k++)
                if (cubettes[i][j][k])
                    retval += (long long)(x_seam[i + 1] - x_seam[i]) * (y_seam[j + 1] - y_seam[j]) * (z_seam[k + 1] - z_seam[k]);

    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 587097);
    assert(part2 == 1359673068597669);
    return 0;
}
