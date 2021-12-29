#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <map>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

const auto num_spaces = 23;

std::array<std::array<std::vector<int>, num_spaces>, num_spaces> path;
std::array<std::array<int, num_spaces>, num_spaces> dist;

void build_paths()
{
    static const std::pair<int, int> index_to_row_col[] =
    {
        {1,1}, {1,2}, {1,4}, {1,6}, {1,8}, {1,10}, {1,11}, // hallway
        {2,3}, {2,5}, {2,7}, {2,9}, // top burrow
        {3,3}, {3,5}, {3,7}, {3,9}, // next burrow
        {4,3}, {4,5}, {4,7}, {4,9}, // next burrow
        {5,3}, {5,5}, {5,7}, {5,9}  // bottom burrow
    };

    std::map<std::pair<int, int>, int> to_index;
    for (auto i = 0; i < num_spaces; i++)
        to_index[index_to_row_col[i]] = i;

    for (auto from = 0; from < num_spaces; from++)
        for (auto to = 0; to < num_spaces; to++)
        {
            auto from_rc = index_to_row_col[from], to_rc = index_to_row_col[to];

            // moving hallway-to-hallway never allowed
            if (from_rc.first == 1 && to_rc.first == 1)
                continue;

            // moving within same burrow never allowed
            if (from_rc.first > 1 && from_rc.second == to_rc.second)
                continue;

            while (from_rc != to_rc)
            {
                if ((from_rc.first > 1 && from_rc.second != to_rc.second) || (to_rc.first < from_rc.first))
                    from_rc.first--;
                else if (from_rc.first == 1 && to_rc.second < from_rc.second)
                    from_rc.second--;
                else if (from_rc.first == 1 && to_rc.second > from_rc.second)
                    from_rc.second++;
                else
                    from_rc.first++;

                dist[from][to]++;

                if (from_rc != to_rc)
                {
                    // don't include spaces that are never stopping points
                    auto it = to_index.find(from_rc);
                    if (it != to_index.end())
                        path[from][to].push_back(it->second);
                }
            }
        }
}

auto read_input_and_initialize(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, retval = ".......";

    std::getline(file, line); std::getline(file, line);

    for (auto i = 0; i < 2; i++)
    {
        std::getline(file, line);
        retval.push_back(line[3]);
        retval.push_back(line[5]);
        retval.push_back(line[7]);
        retval.push_back(line[9]);
    }

    build_paths();
    return retval;
}

auto burrow_has_wrong_letters(const std::string &s, char letter)
{
    assert(isalpha(letter));
    for (auto index = 7 + letter - 'A'; index < s.length(); index += 4)
        if (s[index] != letter && s[index] != '.')
            return true;

    return false;
}

auto is_deeper_burrow_spot_free(const std::string &s, int index)
{
    while ((index += 4) < s.length())
        if (s[index] == '.')
            return true;

    return false;
}

auto letter_cost(char letter)
{
    static const int cost[] = { 1, 10, 100, 1000 };
    assert(isalpha(letter));
    return cost[letter - 'A'];
}

auto compute_projected_cost(const std::string &s, const std::string &end)
{
    auto retval = 0;
    for (auto i = 0; i < s.length(); i++)
        if (isalpha(s[i]) && s[i] != end[i])
            retval += dist[i][7 + s[i] - 'A'] * letter_cost(s[i]);

    return retval;
}

auto is_path_free(const std::string &s, int a, int b)
{
    for (auto i : path[a][b])
        if (s[i] != '.')
            return false;

    return true;
}

auto do_part(std::string pos, bool part2)
{
    struct State
    {
        int cost, projected;
        std::string pos;

        bool operator<(const State &other) const
        {
            if (projected > other.projected) return true;
            if (projected < other.projected) return false;
            if (cost > other.cost) return true;
            if (cost < other.cost) return false;
            return pos > other.pos;
        }
    };

    std::priority_queue<State, std::vector<State>> q;
    std::set<std::pair<std::string, int>> seen;
    auto min_cost = INT_MAX;

    std::string start = part2 ? pos.insert(11, "DCBADBAC") : pos;
    std::string end = part2 ? ".......ABCDABCDABCDABCD" : ".......ABCDABCD";

    q.push({ 0, compute_projected_cost(start, end), start });

    while (!q.empty())
    {
        State state = q.top();
        q.pop();

        if (state.projected >= min_cost)
            continue;

        auto pos_cost = make_pair(state.pos, state.cost);
        if (seen.find(pos_cost) != seen.end())
            continue;

        seen.insert(pos_cost);

        if (state.pos == end)
        {
            min_cost = std::min(min_cost, state.cost);
            continue; 
        }

        for (auto from = 0; from <= end.length(); from++)
        {
            if (!isalpha(state.pos[from]))
                continue;

            // don't move out of a finished burrow
            if (state.pos[from] == end[from] && !burrow_has_wrong_letters(state.pos, state.pos[from]))
                continue;

            for (auto to = 0; to <= end.length(); to++)
            {
                if (state.pos[to] != '.')
                    continue;

                auto d = dist[from][to];
                if (d == 0 || !is_path_free(state.pos, from, to))
                    continue;

                // don't enter wrong burrow, nor one with wrong letters, nor stop before end of one
                if (to > 6 &&
                    (state.pos[from] != end[to] ||
                     burrow_has_wrong_letters(state.pos, end[to]) ||
                     is_deeper_burrow_spot_free(state.pos, to)))
                {
                    continue;
                }

                State new_state = state;
                std::swap(new_state.pos[from], new_state.pos[to]);
                new_state.cost = state.cost + d * letter_cost(state.pos[from]);
                new_state.projected = new_state.cost + compute_projected_cost(new_state.pos, end);
                q.push(new_state);
            }
        }
    }

    return min_cost;
}

int main()
{
    auto input = read_input_and_initialize("input.txt");

    auto part1 = do_part(input, false);
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = do_part(input, true);
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 14371);
    assert(part2 == 40941);
    return 0;
}
