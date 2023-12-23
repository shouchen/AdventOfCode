#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <cassert>

// Note: This is not a general solution, but one based on visual inspection of
// the input. It will not work on the example (test) data given with the problem
// since the assumptions made about the input data are not true there.

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

// This common function assumes we have an infinite lattice of grids like part
// two of the problem, but that fact never comes into play in part one since the
// starting point is at the exact center (65, 65) of the grid, and the single
// grid can never be escaped in part one with only 64 steps.

auto solve_one(int steps)
{
    struct Elem { int steps, row, col; };

    auto n = int(grid.size()), m = int(grid[0].size());
    std::set<std::pair<int, int>> seen;
    std::vector<std::pair<int, int>> ret;
    std::queue<Elem> q;

    q.push({ steps, s_row, s_col });
    seen.insert({ s_row, s_col });

    while (!q.empty())
    {
        auto x = q.front().row, y = q.front().col;
        steps = q.front().steps;
        q.pop();

        if (steps % 2 == 0)
            ret.push_back({ x, y });

        if (steps == 0)
            continue;

        for (auto i = 0; i < 4; i++)
        {
            static const int dx[] = { 0, 1, 0, -1 }, dy[] = { 1, 0, -1, 0 };

            auto nx = x + dx[i], ny = y + dy[i];
            auto nnx = nx, nny = ny;

            // adjust nx and ny, with wrap-arounds
            nx = (nx + n) % n, 
            ny = (ny + m) % m;

            if (nx < 0) nx += n;
            if (ny < 0) ny += m;

            if (grid[nx][ny] == '#' || seen.find({nnx, nny}) != seen.end())
                continue;

            seen.insert({ nnx, nny });
            q.push({ steps - 1, nnx, nny });
        }
    }

    return int(ret.size());
}

auto do_part1()
{
    return solve_one(64);
}

auto do_part2()
{
    // This part assumes that the input grid is square (in this case, 131x131),
    // and that the starting point is in the exact center (65, 65). It assumes
    // that there is no obstacle in that initial row or column, which implies
    // that there is a straight path to all neighboring grids in the lattice.
    // Another assumption is that the grid is not a dense maze, but a relatively
    // sparse bunch of obstacles, which means that within the number of steps
    // needed to cover the start point to the remotest corner, all interior
    // coverage is also achieved. Lastly, it relies on the number of steps being
    // an integer multiple of 131 (plus 65). All this means we can find a
    // pattern in f(0 * 131 + 65), f(1 * 131 + 65), f(2 * 131 + 65),... and
    // extrapolate to a solution. Three values of f() are required since this
    // will be quadratic (we are expanding in the up/down, left/right
    // directions, not just linearly).

    auto target = 26501365LLU;
    auto grid_size = int(grid.size());
    auto center_grid = grid_size / 2;

    auto a0 = solve_one(0 * grid_size + center_grid);
    auto a1 = solve_one(1 * grid_size + center_grid);
    auto a2 = solve_one(2 * grid_size + center_grid);

    // Solve for the quadratic coefficients using method of Day 9
    auto b0 = a0;
    auto b1 = a1 - a0;
    auto b2 = a2 - a1;

    // Extrapolate out to the large number of steps using that polynomial
    auto x = target / grid_size;
    return b0 + b1 * x + (x * (x - 1) / 2) * (b2 - b1);
}

int main()
{
    read_grid("test.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 3677);
    assert(part2 == 609585229256084);
    return 0;
}
