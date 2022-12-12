#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <queue>
#include <cassert>

using RowCol = std::pair<int, int>;

std::vector<std::string> height;
RowCol s_pos, e_pos;

void replace_letter(std::string &line, char letter, char new_letter, RowCol &pos)
{
    auto p = line.find(letter);
    if (p == std::string::npos)
        return;

    line[p] = new_letter;
    pos = std::make_pair(height.size(), p);
}

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (getline(file, line))
    {
        replace_letter(line, 'S', 'a', s_pos);
        replace_letter(line, 'E', 'z', e_pos);
        height.push_back(line);
    }
}

auto is_inside_grid(const RowCol &rc)
{
    return rc.first >= 0 && rc.first < height.size() && rc.second >= 0 && rc.second < height[0].length();
}

auto new_do_part(bool part2)
{
    static const std::array<RowCol, 4> deltas{
        std::make_pair(-1, 0), std::make_pair(1, 0), std::make_pair(0, -1), std::make_pair(0, 1)
    };

    std::map<RowCol, int> dist;
    dist[e_pos] = 0;
    auto best = INT_MAX;

    std::queue<RowCol> q;
    q.push(e_pos);

    while (!q.empty())
    {
        RowCol curr = q.front();
        q.pop();
        
        for (auto &delta : deltas)
        {
            auto next = std::make_pair(curr.first + delta.first, curr.second + delta.second);
            auto next_dist = dist[curr] + 1;

            if (is_inside_grid(next) && height[curr.first][curr.second] - height[next.first][next.second] <= 1 &&
                (dist.find(next) == dist.end() || next_dist < dist[next]))
            {
                dist[next] = next_dist;

                if (part2 && height[next.first][next.second] == 'a' || next == s_pos)
                    best = std::min(best, next_dist);

                q.push(next);
            }
        }
    }

    return best;
}

int main()
{
    read_input("input.txt");

    auto part1 = new_do_part(false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = new_do_part(true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 408);
    assert(part2 == 399);
    return 0;
}
