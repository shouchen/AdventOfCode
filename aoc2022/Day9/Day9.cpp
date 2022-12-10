#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cstdlib>
#include <cassert>

inline auto signum(int x) { return (x == 0) ? 0 : ((x < 0) ? -1 : 1); }

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
                auto diff_row = pos[i - 1].first - pos[i].first;
                auto diff_col = pos[i - 1].second - pos[i].second;

                if (abs(diff_row) > 1 || abs(diff_col) > 1)
                {
                    pos[i].first += signum(diff_row);
                    pos[i].second += signum(diff_col);
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
