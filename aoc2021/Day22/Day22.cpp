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
    int x_min, x_max, y_min, y_max, z_min, z_max;
};

std::vector<InputStep> steps;

void read_input(const std::string &filename)
{
    std::vector<int> x_seam, y_seam, z_seam;

    std::string s, on_off;
    char x = 'x', y = 'y', z = 'z', equals = '=', dot = '.', comma = ',';

    std::ifstream file(filename);

    InputStep step;
    while (file >> on_off >> x >> equals >> step.x_min >> dot >> dot >> step.x_max >> comma >>
        y >> equals >> step.y_min >> dot >> dot >> step.y_max >> comma >>
        z >> equals >> step.z_min >> dot >> dot >> step.z_max)
    {
        step.on = on_off == "on";
        steps.push_back(step);
    }
}

auto do_part1(void)
{
    std::set<std::tuple<int, int, int>> grid;
    for (auto &step : steps)
        if (std::abs(step.x_min) <= 50 && std::abs(step.y_min) <= 50 && std::abs(step.z_min) <= 50)
            for (auto x = step.x_min; x <= step.x_max; x++)
                for (auto y = step.y_min; y <= step.y_max; y++)
                    for (auto z = step.z_min; z <= step.z_max; z++)
                        if (step.on)
                            grid.insert(std::make_tuple(x, y, z));
                        else
                            grid.erase(std::make_tuple(x, y, z));

    return grid.size();
}

auto do_part2()
{
    // Find all the "seams" for each axis (x, y, z boundaries where on/off could possibly change)
    std::vector<int> x_seam, y_seam, z_seam;
    for (auto &step : steps)
    {
        x_seam.push_back(step.x_min); x_seam.push_back(step.x_max + 1);
        y_seam.push_back(step.y_min); y_seam.push_back(step.y_max + 1);
        z_seam.push_back(step.z_min); z_seam.push_back(step.z_max + 1);
    }

    // Sort the seams on each axis
    sort(x_seam.begin(), x_seam.end());
    sort(y_seam.begin(), y_seam.end());
    sort(z_seam.begin(), z_seam.end());

    // Use the seams to partition 3D space, where each point in the partition has same on/off
    std::vector<std::vector<std::vector<bool>>> cubettes(
        x_seam.size(),
        std::vector<
            std::vector<bool>>(
                y_seam.size(),
                std::vector<bool>(
                    z_seam.size())));

    // Play back the input steps to determine the on/off for each partition
    for (auto &step : steps)
    {
        auto x_min = lower_bound(x_seam.begin(), x_seam.end(), step.x_min) - x_seam.begin();
        auto x_max = lower_bound(x_seam.begin(), x_seam.end(), step.x_max + 1) - x_seam.begin();

        auto y_min = lower_bound(y_seam.begin(), y_seam.end(), step.y_min) - y_seam.begin();
        auto y_max = lower_bound(y_seam.begin(), y_seam.end(), step.y_max + 1) - y_seam.begin();

        auto z_min = lower_bound(z_seam.begin(), z_seam.end(), step.z_min) - z_seam.begin();
        auto z_max = lower_bound(z_seam.begin(), z_seam.end(), step.z_max + 1) - z_seam.begin();

        for (auto i = x_min; i < x_max; i++)
            for (auto j = y_min; j < y_max; j++)
                for (auto k = z_min; k < z_max; k++)
                    cubettes[i][j][k] = step.on;
    }

    // For all partitions that are "on", sum the volume of points they contain
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
    read_input("input.txt");

    auto part1 = do_part1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 587097);
    assert(part2 == 1359673068597669);
    return 0;
}
