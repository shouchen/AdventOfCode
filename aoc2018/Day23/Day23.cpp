#include "stdafx.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include "Nanobot.h"

auto get_locations_reachable_by_most_bots(std::vector<Nanobot> &nanobots, std::vector<Coordinates> &locs, long long buffer = 0)
{
    std::vector<Coordinates> next_locs;
    auto max_bots = 0LL;

    for (auto &loc : locs)
    {
        auto num_bots = std::count_if(
            nanobots.begin(), nanobots.end(),
            [&](const Nanobot &n) { return n.reaches(loc, buffer); });

        if (num_bots > max_bots)
        {
            max_bots = num_bots;
            next_locs.clear();
            next_locs.push_back(loc);
        }
        else if (num_bots == max_bots && num_bots != 0)
            next_locs.push_back(loc);
    }

    return next_locs;
}

void sort_and_remove_dups(std::vector<Coordinates> &locs)
{
    std::sort(locs.begin(), locs.end());
    auto last = std::unique(locs.begin(), locs.end());
    locs.erase(last, locs.end());
}

void initialize_part2(std::vector<Nanobot> &nanobots, long long &scale, std::vector<Coordinates> &locs)
{
    Coordinates min, max;

    for (auto &n : nanobots)
    {
        min.x = std::min(min.x, n.loc.x - n.r);
        max.x = std::max(max.x, n.loc.x + n.r + 1);
        min.y = std::min(min.y, n.loc.y - n.r);
        max.y = std::max(max.y, n.loc.y + n.r + 1);
        min.z = std::min(min.z, n.loc.z - n.r);
        max.z = std::max(max.z, n.loc.z + n.r + 1);
    }

    scale = 1LL << static_cast<int>(std::log2(max.x - min.x + max.y - min.y + max.z - min.z) + 1);

    min = { (min.x / scale) * scale, (min.y / scale) * scale,  (min.z / scale) * scale };
    max = { (max.x / scale + 1) * scale, (max.y / scale + 1) * scale,  (max.z / scale + 1) * scale };
    auto num_slices = Coordinates{ (max.x - min.x) / scale, (max.y - min.y) / scale, (max.z - min.z) / scale };

    for (auto i = 0; i < num_slices.x; i++)
        for (auto j = 0; j < num_slices.y; j++)
            for (auto k = 0; k < num_slices.z; k++)
                locs.push_back({ min.x + i * scale, min.y + j * scale, min.z + k * scale });

    locs = get_locations_reachable_by_most_bots(nanobots, locs, scale);
    scale >>= 1;
}

auto read_input(const std::string &filename)
{
    std::ifstream file(filename);
    Nanobot n;

    std::vector<Nanobot> nanobots;
    while (file >> n)
        nanobots.push_back(n);

    return nanobots;
}

auto do_part1(std::vector<Nanobot> &nanobots)
{
    Nanobot *strongest = nullptr;
    for (auto &nanobot : nanobots)
        if (!strongest || nanobot.r > strongest->r)
            strongest = &nanobot;

    auto count = 0U;
    for (auto &nanobot : nanobots)
        if (strongest->reaches(nanobot.loc))
            count++;

    return count;
}

auto do_part2(std::vector<Nanobot> &nanobots)
{
    auto scale = 0LL;
    std::vector<Coordinates> locs;

    initialize_part2(nanobots, scale, locs);

    while (scale >>= 1)
    {
        std::vector<Coordinates> next_locs;

        for (auto &loc : locs)
            for (auto i = -scale; i <= scale; i += scale)
                for (auto j = -scale; j <= scale; j += scale)
                    for (auto k = -scale; k <= scale; k += scale)
                        next_locs.push_back({ loc.x + i, loc.y + j, loc.z + k });

        sort_and_remove_dups(next_locs);
        locs = get_locations_reachable_by_most_bots(nanobots, next_locs, scale);
        next_locs.clear();
    }

    auto min_distance = std::numeric_limits<long long>::max();
    for (auto &loc : get_locations_reachable_by_most_bots(nanobots, locs))
        min_distance = std::min(min_distance, loc.dist_from_origin());

    return min_distance;
}

int main()
{
    auto nanobots = read_input("input.txt");

    auto part1 = do_part1(nanobots);
    auto part2 = do_part2(nanobots);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 780);
    assert(part2 == 110841112);

    return 0;
}
