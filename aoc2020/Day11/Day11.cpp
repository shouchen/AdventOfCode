#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

auto count_occupied(std::vector<std::string> &d, int row, int col, bool part2)
{
    auto count = 0;
    for (auto i = -1; i <= 1; i++)
        for (auto j = -1; j <= 1; j++)
            if (i || j)
            {
                auto r = row, c = col;
                do
                {
                    r += i, c += j;
                    if (r < 0 || r >= d.size() || c < 0 || c >= d[0].length() || d[r][c] == 'L')
                        break;

                    if (d[r][c] == '#')
                    {
                        count++;
                        break;
                    }
                } while (part2);
            }

    return count;
}

auto do_part(bool part2)
{
    std::ifstream file("input.txt");
    std::string s;
    std::vector<std::string> input;

    while (file >> s)
        input.push_back(s);

    for (;;)
    {
        auto snapshot = input;
        auto changed = false;
        auto pounds = 0;

        for (auto i = 0; i < input.size(); i++)
            for (auto j = 0; j < input[i].length(); j++)
            {
                auto &cell = input[i][j];
                if (cell == '.')
                    continue;

                auto occupied = count_occupied(snapshot, i, j, part2);
                if (cell == 'L' && occupied == 0)
                {
                    cell = '#';
                    changed = true;
                }
                else if (cell == '#' && occupied >= (part2 ? 5 : 4))
                {
                    cell = 'L';
                    changed = true;
                }

                if (cell == '#')
                    pounds++;
            }

        if (!changed)
            return pounds;
    }
}

int main()
{
    auto part1 = do_part(false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part(true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 2289);
    assert(part2 == 2059);
    return 0;
}
