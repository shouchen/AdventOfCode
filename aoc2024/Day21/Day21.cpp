#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <queue>
#include <string>
#include <ranges>
#include <cassert>

struct Memo
{
    char start, end;
    int keypad_idx;

    bool operator == (const Memo &m) const {
        return start == m.start && end == m.end && keypad_idx == m.keypad_idx;
    }
};

struct MemoHasher
{
    std::size_t operator () (const Memo &m) const {
        return int(m.start) + int(m.end) * 100 + m.keypad_idx * 10000;
    }
};

using MemoMap = std::unordered_map<Memo, long long int, MemoHasher>;
using AllBestKeyboardPaths = std::map<std::pair<char, char>, std::vector<std::string>>;

struct Point
{
    int row, col;
    bool operator == (const Point &p) const { return row == p.row && col == p.col; }
};

constexpr std::array<Point, 4> moves{ Point(-1, 0), Point(1, 0), Point(0, -1), Point(0, 1) };
const std::array<char, 4> arrows{ { '^', 'v', '<', '>' } };

struct Comparator
{
    bool operator()(
        const std::tuple<Point, std::vector<Point>, int> &p1,
        const std::tuple<Point, std::vector<Point>, int> &p2) const {
        return std::get<2>(p1) > std::get<2>(p2);
    }
};

const std::vector<std::string> num_keypad{
    { '7', '8', '9'},
    { '4', '5', '6'},
    { '1', '2', '3'},
    { ' ', '0', 'A'}
};

const std::vector<std::string> dir_keypad{
    { ' ', '^', 'A'},
    { '<', 'v', '>'}
};

auto convert_path_to_arrows(const std::vector<Point> &path)
{
    std::string path_arrows;

    for (auto i = 1; i < path.size(); i++)
    {
        auto dr = path[i].row - path[i - 1].row, dc = path[i].col - path[i - 1].col;

        for (const auto &[idx, move] : std::views::enumerate(moves))
            if (move.row == dr && move.col == dc)
            {
                path_arrows.push_back(arrows[idx]);
                break;
            }
    }

    return path_arrows;
}

// find all best paths between a given pair of points (with ' ' disallowed)
auto find_best_paths(const std::vector<std::string> &map, const Point &start, const Point &end)
{
    using State = std::tuple<Point, std::vector<Point>, int>;

    std::priority_queue<State, std::vector<State>, Comparator> pq;
    pq.push({ start, {}, 0 });

    std::vector<std::vector<Point>> best_paths;
    auto best_score = std::numeric_limits<int>::max();

    while (!pq.empty())
    {
        auto [current, path, score] = pq.top(); pq.pop();

        if (score > best_score)
            break;

        if (std::find(path.begin(), path.end(), current) != path.end())
            continue;

        path.push_back(current);

        if (current.row == end.row && current.col == end.col)
        {
            best_score = score;
            best_paths.push_back(path);
        }

        for (auto &move : moves)
        {
            auto new_row = current.row + move.row, new_col = current.col + move.col;
            Point new_point{ new_row, new_col };

            if (new_row >= 0 && new_row < map.size() && new_col >= 0 && new_col < map[new_row].size() &&
                map[new_row][new_col] != ' ' &&
                std::find(path.begin(), path.end(), new_point) == path.end())
            {
                pq.push({ new_point, path, score + 1 });
            }
        }
    }

    return best_paths;
}

auto compute_all_best_paths(const std::vector<std::string> &keypad)
{
    AllBestKeyboardPaths keypad_paths;

    for (const auto &[row_l1_i, row_l1] : std::views::enumerate(keypad))
        for (const auto &[col_l1_i, ele_l1] : std::views::enumerate(row_l1))
            if (ele_l1 != ' ')
                for (const auto &[row_l2_i, row_l2] : std::views::enumerate(keypad))
                    for (const auto &[col_l2_i, ele_l2] : std::views::enumerate(row_l2))
                        if (ele_l2 != ' ')
                        {
                            const auto best_paths = find_best_paths(
                                keypad, { int(row_l1_i), int(col_l1_i) }, { int(row_l2_i), int(col_l2_i) });

                            for (const auto &best_path : best_paths)
                                keypad_paths[{ele_l1, ele_l2}].push_back(convert_path_to_arrows(best_path));
                        }

    return keypad_paths;
}

long long recur(const std::string &sequence, int keypad_idx, AllBestKeyboardPaths &dir_keypad_paths, MemoMap &memo)
{
    if (keypad_idx == 0)
        return sequence.size();

    auto retval = 0LL;
    auto prev = 'A';

    for (auto curr : sequence)
    {
        Memo m{ prev, curr, keypad_idx };

        if (memo.contains(m))
        {
            retval += memo[m];
        }
        else
        {
            auto answer = LLONG_MAX;
            for (const auto &v : dir_keypad_paths[{ prev, curr }])
                answer = std::min(answer, recur(v + 'A', keypad_idx - 1, dir_keypad_paths, memo));

            retval += memo[m] = answer;
        }

        prev = curr;
    }

    return retval;
}

auto solve(const std::string &filename, int highest_keypad_idx)
{
    auto num_keypad_paths = compute_all_best_paths(num_keypad), dir_keypad_paths = compute_all_best_paths(dir_keypad);
    MemoMap memo;

    std::ifstream file(filename);
    std::string code;
    auto retval = 0LL;

    while (file >> code)
    {
        auto best_length = 0LL;
        auto prev_key = 'A';

        for (auto key : code)
        {
            auto best_length_consecutive = LLONG_MAX;
            for (const auto &v : num_keypad_paths[{ prev_key, key }])
                best_length_consecutive = std::min(best_length_consecutive, recur(v + 'A', highest_keypad_idx - 1, dir_keypad_paths, memo));

            best_length += best_length_consecutive;
            prev_key = key;
        }

        auto num_val = std::stoi(code);
        retval += best_length * num_val;
    }

    return retval;
}

int main(int argc, char *argv[])
{
    auto part1 = solve("input.txt", 3);
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 184180);

    auto part2 = solve("input.txt", 26);
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 231309103124520);

    return 0;
}