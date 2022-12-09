#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cstdlib>
#include <cassert>

auto do_part(const std::string &filename, int n)
{
    auto pos = std::vector<std::pair<int, int>>(n);
    std::set<std::pair<int, int>> visited;

    std::ifstream file(filename);
    auto dir = ' ';
    auto times = 0;

    while (file >> dir >> times)
    {
        while (times--)
        {
            switch (dir)
            {
                case 'U': pos[0].first--; break;
                case 'D': pos[0].first++; break;
                case 'L': pos[0].second--; break;
                case 'R': pos[0].second++; break;
            }

            for (auto i = 1; i < n; i++)
            {
                auto drow = pos[i].first - pos[i - 1].first;
                auto dcol = pos[i].second - pos[i - 1].second;

                if (abs(drow) == 2 && dcol == 0)
                    pos[i].first -= drow / 2;
                else if (drow == 0 && abs(dcol) == 2)
                    pos[i].second -= dcol / 2;
                else if (abs(drow) == 2 || abs(dcol) == 2)
                {
                    if (drow > 0) pos[i].first--;
                    if (drow < 0) pos[i].first++;
                    if (dcol > 0) pos[i].second--;
                    if (dcol < 0) pos[i].second++;
                }
            }

            visited.insert(pos[n - 1]);
        }
    }

    return visited.size();
}

int main()
{
    auto part1 = do_part("input.txt", 2);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part("input.txt", 10);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 6090);
    assert(part2 == 2566);
    return 0;
}
