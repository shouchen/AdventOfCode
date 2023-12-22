#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <cassert>

std::vector<std::string> grid;
auto s_row = 0, s_col = 0;

void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        auto p = int(line.find('S'));
        if (p != std::string::npos)
            s_row = int(grid.size()), s_col = p;

        grid.push_back(line);
    }
}

auto solve(int steps, bool multiple = false)
{
    struct Elem { int steps, row, col; };

    static const int dx[] = { 0, 1, 0, -1 }, dy[] = { 1, 0, -1, 0 };

    auto n = int(grid.size()), m = int(grid[0].size());

    std::set<std::pair<int, int>> seen;
    std::vector<std::pair<int, int>> ret;
    std::queue<Elem> q;

    q.push({ steps, s_row, s_col });
    seen.insert(std::make_pair(s_row, s_col));

    while (!q.empty())
    {
        steps = q.front().steps;
        auto x = q.front().row, y = q.front().col;
        q.pop();

        if (steps % 2 == 0)
            ret.push_back(std::make_pair(x, y));

        if (steps == 0)
            continue;

        for (auto i = 0; i < 4; i++)
        {
            auto nx = x + dx[i], ny = y + dy[i];
            auto nnx = nx, nny = ny;

            if (multiple)
            {
                while (nx < 0) nx += n;
                while (ny < 0) ny += m;

                nx = (nx + n) % n;
                ny = (ny + m) % m;
            }

            if (nx < 0 || nx >= n || ny < 0 || ny >= m)
                continue;

            if (grid[nx][ny] == '#')
                continue;

            if (seen.find(std::make_pair(nnx, nny)) != seen.end())
                continue;

            seen.insert(std::make_pair(nnx, nny));
            q.push({ steps - 1, nnx, nny });
        }
    }

    return ret.size();
}

long long do_part2(long long unsigned target)
{
    // assert grid is square
    // asserts starts in middle
    // assume init row, col is otherwise free
    // assume not dense maze

    // Looking at the specific input data, make a few assumptions: (1) input
    // grid is square, (2) straight vertical and horizontal lines are open from
    // S position. So the result should be quadratic: 
    //     (rem), (rem + grid_size), (rem + 2 * grid_size), ...

    auto sol = 0LLU;

    auto steps = target - 65LLU;

    // Three numbers in the series is enough to extrapolate to large vales if
    // this is quadratic. Similar to part1.
    auto a0 = solve(0 * 131 + 65, true);
    auto a1 = solve(1 * 131 + 65, true);
    auto a2 = solve(2 * 131 + 65, true);

    // Solve for the quadratic coefficients using method of Day 9
    auto b0 = a0;
    auto b1 = a1 - a0;
    auto b2 = a2 - a1;

    auto x = steps / 131;

    // Extrapolate
    return ((b0 + b1 * x + (x * (x - 1) / 2) * (b2 - b1)));
}

int main()
{
    read_grid("input.txt");

    auto part1 = solve(64);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2(26501365LLU);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 3677);
    assert(part2 == 609585229256084);
    return 0;
}
