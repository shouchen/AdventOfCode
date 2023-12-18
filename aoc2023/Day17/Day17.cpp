#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <cassert>

std::vector<std::string> grid;

void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
        grid.push_back(line);
}

auto do_part(bool part2)
{
    static const int dx[] = { 0, 1, 0, -1 }, dy[] = { 1, 0, -1, 0 };
    int N = int(grid.size()), M = int(grid[0].size());

    auto dijkstra = [&]() -> auto
    {
        std::vector<std::vector<std::vector<std::vector<int>>>> dist(
            N, std::vector<std::vector<std::vector<int>>>(
                M, std::vector<std::vector<int>>(
                    4, std::vector<int>(11, INT_MAX))));

        using T = std::array<int, 4>;
        std::priority_queue<std::pair<int, T>, std::vector<std::pair<int, T>>, std::greater<std::pair<int, T>>> q;

        for (auto d = 0; d < 4; d++)
        {
            dist[0][0][d][0] = 0;
            q.push({ 0, {0, 0, d, 0} });
        }

        auto valid = [&](int x, int y)
        {
            return x >= 0 && x < N && y >= 0 && y < M;
        };

        auto getAdj1 = [&](int x, int y, int d, int s) -> auto
        {
            std::vector<T> retval;

            auto d1 = (d + 3) % 4;
            retval.push_back({ x + dx[d1], y + dy[d1], d1, 1 });

            auto d2 = (d + 1) % 4;
            retval.push_back({ x + dx[d2], y + dy[d2], d2, 1 });

            if (s < 3)
                retval.push_back({ x + dx[d], y + dy[d], d, s + 1 });

            return retval;
        };

        auto getAdj2 = [&](int x, int y, int d, int s) -> auto
        {
            std::vector<T> retval;

            if (s >= 4)
            {
                auto d1 = (d + 3) % 4;
                retval.push_back({ x + dx[d1], y + dy[d1], d1, 1 });

                auto d2 = (d + 1) % 4;
                retval.push_back({ x + dx[d2], y + dy[d2], d2, 1 });
            }
            if (s < 10)
                retval.push_back({ x + dx[d], y + dy[d], d, s + 1 });

            return retval;
        };

        while (!q.empty())
        {
            auto [d, u] = q.top(); q.pop();
            auto [x, y, dir, s] = u;

            if (d > dist[x][y][dir][s])
                continue;

            auto adj = part2 ? getAdj2(x, y, dir, s) : getAdj1(x, y, dir, s);
            for (auto [nx, ny, nd, ns] : adj)
            {
                if (!valid(nx, ny))
                    continue;

                int weight = grid[nx][ny] - '0';
                if (dist[nx][ny][nd][ns] > dist[x][y][dir][s] + weight)
                {
                    dist[nx][ny][nd][ns] = dist[x][y][dir][s] + weight;
                    q.push({ dist[nx][ny][nd][ns], {nx, ny, nd, ns} });
                }
            }
        }
        return dist;
    };

    auto dist = dijkstra();
    auto min_s = part2 ? 4 : 0, max_s = part2 ? 10 : 3;
    auto ans = INT_MAX;

    for (auto d = 0; d < 4; d++)
        for (auto s = min_s; s <= max_s; s++)
            ans = std::min(ans, dist[N - 1][M - 1][d][s]);

    return ans;
}

int main()
{
    read_grid("input.txt");

    auto part1 = do_part(false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part(true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 928);
    assert(part2 == 1104);
    return 0;
}
