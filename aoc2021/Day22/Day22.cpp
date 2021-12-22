#include <iostream>
#include <fstream>
#include <array>
#include <set>
#include <vector>
#include <algorithm>
#include <cassert>

struct InputStep
{
    bool on;
    std::pair<int, int> x, y, z;
};

std::vector<InputStep> steps;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string on_off;
    auto x = 'x', y = 'y', z = 'z', equals = '=', dot = '.', comma = ',';

    InputStep step;
    while (file >> on_off >> x >> equals >> step.x.first >> dot >> dot >> step.x.second >> comma >>
        y >> equals >> step.y.first >> dot >> dot >> step.y.second >> comma >>
        z >> equals >> step.z.first >> dot >> dot >> step.z.second)
    {
        step.on = on_off == "on";
        steps.push_back(step);
    }
}

auto do_part1(void)
{
    std::set<std::tuple<int, int, int>> grid;
    for (auto &step : steps)
        if (std::abs(step.x.first) <= 50 && std::abs(step.y.first) <= 50 && std::abs(step.z.first) <= 50)
            for (auto x = step.x.first; x <= step.x.second; x++)
                for (auto y = step.y.first; y <= step.y.second; y++)
                    for (auto z = step.z.first; z <= step.z.second; z++)
                        if (step.on)
                            grid.insert(std::make_tuple(x, y, z));
                        else
                            grid.erase(std::make_tuple(x, y, z));

    return grid.size();
}

auto do_part2()
{
    // Find all the "seams" for each axis (x, y, z boundaries where on/off could possibly change).
    std::vector<int> x_seam, y_seam, z_seam;
    for (auto &step : steps)
    {
        x_seam.push_back(step.x.first); x_seam.push_back(step.x.second + 1);
        y_seam.push_back(step.y.first); y_seam.push_back(step.y.second + 1);
        z_seam.push_back(step.z.first); z_seam.push_back(step.z.second + 1);
    }

    // Sort the seams for each axis
    sort(x_seam.begin(), x_seam.end());
    sort(y_seam.begin(), y_seam.end());
    sort(z_seam.begin(), z_seam.end());

    // Use the seams to partition 3D space, where each point in the partition has same on/off.
    // Create this 3D vector, sized at runtime, to hold on/off for all the partitions.
    std::vector<std::vector<std::vector<bool>>> partitions(
        x_seam.size(),
        std::vector<
            std::vector<bool>>(
                y_seam.size(),
                std::vector<bool>(
                    z_seam.size())));

    // Play back the input steps to determine the on/off for each partition - multiple
    // adjacent partitions may be affected at each step.
    for (auto &step : steps)
    {
        auto x_min = lower_bound(x_seam.begin(), x_seam.end(), step.x.first) - x_seam.begin();
        auto x_max = lower_bound(x_seam.begin(), x_seam.end(), step.x.second + 1) - x_seam.begin();

        auto y_min = lower_bound(y_seam.begin(), y_seam.end(), step.y.first) - y_seam.begin();
        auto y_max = lower_bound(y_seam.begin(), y_seam.end(), step.y.second + 1) - y_seam.begin();

        auto z_min = lower_bound(z_seam.begin(), z_seam.end(), step.z.first) - z_seam.begin();
        auto z_max = lower_bound(z_seam.begin(), z_seam.end(), step.z.second + 1) - z_seam.begin();

        for (auto i = x_min; i < x_max; i++)
            for (auto j = y_min; j < y_max; j++)
                for (auto k = z_min; k < z_max; k++)
                    partitions[i][j][k] = step.on;
    }

    // For all partitions that are "on", sum the volume of points they contain.
    auto retval = 0LL;
    for (auto i = 0; i < x_seam.size() - 1; i++)
        for (auto j = 0; j < y_seam.size() - 1; j++)
            for (auto k = 0; k < z_seam.size() - 1; k++)
                if (partitions[i][j][k])
                    retval += (long long)(x_seam[i + 1] - x_seam[i]) * (y_seam[j + 1] - y_seam[j]) * (z_seam[k + 1] - z_seam[k]);

    return retval;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 587097);
    assert(part2 == 1359673068597669);
    return 0;
}
