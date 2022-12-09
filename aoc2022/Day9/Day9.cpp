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
        for (auto i = 0; i < times; i++)
        {
            // move head
            if (dir == 'U')
                --pos[0].first;
            else if (dir == 'D')
                pos[0].first++;
            else if (dir == 'L')
                --pos[0].second;
            else if (dir == 'R')
                pos[0].second++;

            // check tail move if two steps
            for (auto j = 1; j < n; j++)
            {
                if (pos[j].first - pos[j - 1].first == 2 && pos[j].second == pos[j - 1].second)
                    pos[j].first--;
                else if (pos[j - 1].first - pos[j].first == 2 && pos[j].second == pos[j - 1].second)
                    pos[j].first++;
                else if (pos[j].second - pos[j - 1].second == 2 && pos[j].first == pos[j - 1].first)
                    pos[j].second--;
                else if (pos[j - 1].second - pos[j].second == 2 && pos[j].first == pos[j - 1].first)
                    pos[j].second++;
                else if (abs(pos[j - 1].first - pos[j].first) == 2 || abs(pos[j - 1].second - pos[j].second) == 2)
                {
                    if (pos[j - 1].first < pos[j].first)
                        pos[j].first--;
                    if (pos[j - 1].first > pos[j].first)
                        pos[j].first++;
                    if (pos[j - 1].second < pos[j].second)
                        pos[j].second--;
                    if (pos[j - 1].second > pos[j].second)
                        pos[j].second++;
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
