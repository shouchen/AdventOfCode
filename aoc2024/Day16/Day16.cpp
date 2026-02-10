#include <iostream>
#include <fstream>
#include <vector> 
#include <array>
#include <queue>
#include <unordered_set>
#include <string>
#include <ranges>
#include <cassert>

struct LocDir
{
    int row, col, dir;
    auto operator==(const LocDir &p) const { return row == p.row && col == p.col && dir == p.dir; }
    auto operator<(const LocDir &p) const { return row < p.row || (row == p.row && col < p.col) || (row == p.row && col == p.col && dir < p.dir); }
};

using Grid = std::vector<std::string>;
using LocDirWithScore = std::pair<LocDir, int>;
using BestScoresMap = std::vector<std::vector<std::array<int, 4>>>;

struct SameLocation
{
    auto operator()(const LocDir &lhs, const LocDir &rhs) const { return lhs.row == rhs.row && lhs.col == rhs.col; }
};

struct ScoreComparator
{
    auto operator()(const LocDirWithScore &lhs, const LocDirWithScore &rhs) const { return lhs.second > rhs.second; }
};

struct Hasher { auto operator()(const LocDir &ld) const { return ld.row; } };

constexpr std::array<LocDir, 4> moves{ LocDir(0, 1, 0), LocDir(1, 0, 1), LocDir(0,-1, 2), LocDir(-1, 0, 3) };

auto find_best_scores_for_all_points_with_dirs(const Grid &grid, const LocDir &start, const LocDir &end, const int best_path_score)
{
    BestScoresMap best_cost_map(
        grid.size(),
        std::vector<std::array<int, 4>>(grid[0].size(), { { INT_MIN, INT_MIN, INT_MIN, INT_MIN } })
    );

    std::priority_queue<LocDirWithScore, std::vector<LocDirWithScore>, ScoreComparator> pq;
    std::unordered_set<LocDir, Hasher> visited;

    pq.push({ end, 0 });
    while (!pq.empty())
    {
        auto [current, score] = pq.top(); pq.pop();

        if (visited.contains(current))
            continue;

        visited.insert(current);

        best_cost_map[current.row][current.col][current.dir] = score;
        auto &move = moves[current.dir];

        LocDir new_point{ current.row - move.row, current.col - move.col };
        if (grid[new_point.row][new_point.col] == '#')
            continue;

        for (auto dir = 0; dir < 4; dir++)
        {
            new_point.dir = dir;

            if (current.dir == new_point.dir)
                pq.push({ new_point, score + 1 });
            else if (std::abs(current.dir - new_point.dir) == 2)
                pq.push({ new_point, score + 2001 });
            else
                pq.push({ new_point, score + 1001 });
        }
    }

    return best_cost_map;
}

auto find_best_path_score_and_end_dir(const Grid &grid, const LocDir &start, const LocDir &end)
{
    std::priority_queue<LocDirWithScore, std::vector<LocDirWithScore>, ScoreComparator> pq;
    std::unordered_set<LocDir, Hasher> visited;

    pq.push({ start, 0 });
    auto best_path_score = 0;

    while (!pq.empty())
    {
        auto [current, score] = pq.top(); pq.pop();

        if (visited.contains(current)) continue;
        visited.insert(current);

        if (current.row == end.row && current.col == end.col)
            return std::make_pair(score, current.dir); // score end dir

        for (const auto &move : moves)
        {
            LocDir new_point{ current.row + move.row, current.col + move.col, move.dir };
            if (grid[new_point.row][new_point.col] == '#')
                continue;

            if (current.dir == new_point.dir)
                pq.push({ new_point, score + 1 });
            else if (std::abs(current.dir - new_point.dir) == 2)
                pq.push({ new_point, score + 2001 });
            else
                pq.push({ new_point, score + 1001 });
        }
    }

    return std::make_pair(-1, -1);
}

auto traverse_best_paths(const BestScoresMap &best_scores_map, const Grid &grid, const LocDir &start, const LocDir &end)
{
    std::priority_queue<LocDirWithScore, std::vector<LocDirWithScore>, ScoreComparator> pq;
    std::unordered_set<LocDir, Hasher> visited;

    pq.push({ start, best_scores_map[start.row][start.col][start.dir] });

    while (!pq.empty())
    {
        auto [current, score] = pq.top();
        pq.pop();

        if (visited.contains(current)) continue;
        visited.insert(current);

        if (current.row == end.row && current.col == end.col)
            continue;

        for (const auto &move : moves)
        {
            auto new_row = current.row + move.row, new_col = current.col + move.col;
            if (grid[new_row][new_col] == '#')
                continue;

            if (current.dir == move.dir && best_scores_map[new_row][new_col][move.dir] == score - 1)
                pq.push({ { new_row, new_col, move.dir }, score - 1 });
            else if (std::abs(current.dir - move.dir) == 2 && best_scores_map[new_row][new_col][move.dir] == score - 2001)
                pq.push({ { new_row, new_col, move.dir }, score - 2001 });
            else if ((std::abs(current.dir - move.dir) == 1 || std::abs(current.dir - move.dir) == 3) && best_scores_map[new_row][new_col][move.dir] == score - 1001)
                pq.push({ { new_row, new_col, move.dir }, score - 1001 });
        }
    }

    return visited;
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    Grid grid;
    LocDir start, end;

    while (file >> line)
    {
        auto s = line.find('S'), e = line.find('E');
        if (s != std::string::npos) start = { int(grid.size()), int(s) }, line[s] = '.';
        if (e != std::string::npos) end = { int(grid.size()), int(e) }, line[e] = '.';

        grid.push_back(line);
    }

    // puzzle says we can assume the end position is reached optimally from one direction only
    auto [best_path_score, end_dir] = find_best_path_score_and_end_dir(grid, start, end);
    end.dir = end_dir;

    // continue with part 2
    auto best_score_map = find_best_scores_for_all_points_with_dirs(grid, start, end, best_path_score);
    auto on_best_path = traverse_best_paths(best_score_map, grid, start, end);

    // strips out dupes that are the same location but with different directions
    std::unordered_set<LocDir, Hasher, SameLocation> on_best_path_filtered;
    for (const auto &p : on_best_path)
        on_best_path_filtered.insert(p);

    return std::make_pair(best_path_score, on_best_path_filtered.size());
}

int main(int argc, char *argv[])
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 101492);
    assert(answer.second == 543);
    return 0;
}