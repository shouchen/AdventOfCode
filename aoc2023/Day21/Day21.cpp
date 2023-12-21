#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <cassert>

std::vector<std::string> grid;
auto s_row = -1, s_col = -1;

void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        auto p = line.find('S');
        if (p != std::string::npos)
            s_row = grid.size(), s_col = p;

        grid.push_back(line);
    }
}

auto do_part1(const std::string &filename, int steps)
{
    struct Elem
    {
        int row, col, dist;
    };

    std::set<std::pair<int, int>> reachable;
    std::set<std::pair<std::pair<int, int>, int>> seen;

    std::queue<Elem> q;
    q.push({ s_row, s_col, 0 });

    while (!q.empty())
    {
        auto elem = q.front(); q.pop();
        auto row = elem.row, col = elem.col, dist = elem.dist;

        if (seen.find(std::make_pair(std::make_pair(row, col), dist)) != seen.end())
            continue;
        seen.insert(std::make_pair(std::make_pair(row, col), dist));

        if (grid[row][col] == '#')
            continue;

        if (dist == steps)
        {
            reachable.insert({ row, col });
            continue;
        }

        if (row > 0 && grid[row - 1][col] != '#')
            q.push({ row - 1, col, dist + 1 });
        if (row < grid.size() - 1 && grid[row + 1][col] != '#')
            q.push({ row + 1, col, dist + 1 });
        if (col > 0 && grid[row][col - 1] != '#')
            q.push({ row, col - 1, dist + 1 });
        if (col < grid[0].size() - 1 && grid[row][col + 1] != '#')
            q.push({ row, col + 1, dist + 1 });
    }

    return reachable.size();
}

char get_cell(long long row, long long col)
{
    auto rows = grid.size(), cols = grid[0].size();
    row %= rows, col %= cols;
    if (row < 0) row += rows;
    if (col < 0) col += cols;
    return grid[row][col];
}

int dx[] = { 0, -1, 0, 1 };
int dy[] = { -1, 0, 1, 0 };

long long do_part2(long long target)
{
    auto grid_size = grid.size();
    auto grids = target / grid.size();
    auto rem = target % grid.size();

    // Looking at the specific input data, make a few assumptions: (1) input
    // grid is square, (2) straight vertical and horizontal lines are open from
    // S position. So the result should be quadratic: 
    //     (rem), (rem + grid_size), (rem + 2 * grid_size), ...
    std::vector<long long> sequence;
    std::set<std::pair<long long, long long>> work;
    auto steps = 0;

    work.insert({ s_row, s_col });

    // Three numbers in the series is enough to extrapolate to large vales if
    // this is quadratic. Similar to part1.
    for (auto n = 0; n < 3; n++)
    {
        for (; steps < n * grid_size + rem; steps++)
        {
            std::set<std::pair<long long, long long>> next;

            for (auto coord : work)
                for (auto dir = 0; dir < 4; dir++)
                {
                    auto dest = std::make_pair(coord.first + dy[dir], coord.second + dx[dir]);
                    if (get_cell(dest.first, dest.second) != '#')
                        next.insert(dest);
                }

            work = next;
        }

        sequence.push_back(work.size());
    }

    // Solve for the quadratic coefficients using method of Day 9
    auto c = sequence[0];
    auto a_plus_b = sequence[1] - c;
    auto four_a_plus_two_b = sequence[2] - c;
    auto two_a = four_a_plus_two_b - (2 * a_plus_b);
    auto a = two_a / 2;
    auto b = a_plus_b - a;

    // Extrapolate
    auto n = grids;
    return a * (n * n) + b * n + c;
}

int main()
{
    read_grid("input.txt");

    auto part1 = do_part1("input.txt", 64);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2(26501365LL);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 3677);
    assert(part2 == 609585229256084);
    return 0;
}
