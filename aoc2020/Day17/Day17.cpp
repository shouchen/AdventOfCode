#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <tuple>
#include <algorithm>
#include <cassert>

struct Grid
{
    std::set<std::tuple<int, int, int ,int>> grid;

    inline void insert(int x, int y, int z = 0, int w = 0)
    {
        grid.insert(std::make_tuple(x, y, z, w));
        x_range.first = std::min(x_range.first, x), x_range.second = std::max(x_range.second, x);
        y_range.first = std::min(y_range.first, y), y_range.second = std::max(y_range.second, y);
        z_range.first = std::min(z_range.first, z), z_range.second = std::max(z_range.second, z);
        w_range.first = std::min(w_range.first, w), w_range.second = std::max(w_range.second, w);
    }

    void do_cycle(bool part2)
    {
        auto first_w = part2 ? w_range.first - 1 : 0, last_w = part2 ? w_range.second + 1 : 0;
        auto prev = *this;

        for (auto x = x_range.first - 1; x <= x_range.second + 1; x++)
            for (auto y = y_range.first - 1; y <= y_range.second + 1; y++)
                for (auto z = z_range.first - 1; z <= z_range.second + 1; z++)
                    for (auto w = first_w; w <= last_w; w++)
                    {
                        auto t = std::make_tuple(x, y, z, w);
                        auto neighbors = prev.count_active_neighbors(t);
                        auto found = grid.find(t);

                        if (found != grid.end())
                        {
                            if (neighbors != 2 && neighbors != 3)
                                grid.erase(found);
                        }
                        else if (neighbors == 3)
                        {
                            insert(x, y, z, w);
                        }
                    }
    }

private:
    std::pair<int, int> x_range = std::make_pair(INT_MAX, INT_MIN);
    std::pair<int, int> y_range = std::make_pair(INT_MAX, INT_MIN);
    std::pair<int, int> z_range = std::make_pair(INT_MAX, INT_MIN);
    std::pair<int, int> w_range = std::make_pair(INT_MAX, INT_MIN);

    int count_active_neighbors(std::tuple<int, int, int, int> &t)
    {
        auto count = 0;
        for (auto x = -1; x <= 1; x++)
            for (auto y = -1; y <= 1; y++)
                for (auto z = -1; z <= 1; z++)
                    for (auto w = -1; w <= 1; w++)
                        if (x != 0 || y != 0 || z != 0 || w != 0)
                            if (grid.find(
                                    std::make_tuple(std::get<0>(t) + x, std::get<1>(t) + y,
                                    std::get<2>(t) + z, std::get<3>(t) + w)) != grid.end())
                                count++;

        return count;
    }
};

auto do_part(const std::string &filename, bool part2)
{
    std::ifstream file(filename);
    std::string s;
    auto x = 0, y = 0;
    Grid grid;

    while (file >> s)
    {
        for (auto c : s)
        {
            if (c == '#')
                grid.insert(x, y);

            x++;
        }

        x = 0, y++;
    }

    for (auto i = 0; i < 6; i++)
        grid.do_cycle(part2);

    return grid.grid.size();
}

int main()
{
    auto part1 = do_part("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 209);
    assert(part2 == 1492);
    return 0;
}
