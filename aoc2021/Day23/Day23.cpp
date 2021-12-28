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

std::array<std::array<std::vector<int>, 23>, 23> path;
std::array<std::array<int, 23>, 23> dist;

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
    for (auto i = 0; i < 23; i++)
        to_index[index_to_row_col[i]] = i;

    for (auto from = 0; from < 23; from++)
        for (auto to = 0; to < 23; to++)
        {
            auto a_rc = index_to_row_col[from], b_rc = index_to_row_col[to];

            // hallway-to-hallway never allowed
            if (a_rc.first == 1 && b_rc.first == 1)
                continue;

            // within same burrow never allowed
            if (a_rc.first > 1 && b_rc.first > 1 && a_rc.second == b_rc.second)
                continue;

            while (a_rc != b_rc)
            {
                if ((a_rc.first > 1 && a_rc.second != b_rc.second) || (b_rc.first < a_rc.first))
                    a_rc.first--;
                else if (a_rc.first == 1 && b_rc.second < a_rc.second)
                    a_rc.second--;
                else if (a_rc.first == 1 && b_rc.second > a_rc.second)
                    a_rc.second++;
                else
                    a_rc.first++;

                dist[from][to]++;

                if (a_rc != b_rc)
                {
                    // grid spaces that aren't stopping points are never occupied
                    auto it = to_index.find(a_rc);
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

auto top_burrow(char letter)
{
    assert(isalpha(letter));
    return 7 + letter - 'A';
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
            retval += dist[i][top_burrow(s[i])] * letter_cost(s[i]);

    return retval;
}

auto burrow_has_wrong_letters(const std::string &s, char letter)
{
    assert(isalpha(letter));
    auto tb = top_burrow(letter);
    if (s.length() == 15)
        return (s[tb] != letter && s[tb] != '.') || (s[tb + 4] != letter && s[tb + 4] != '.');
    else
        return
            (s[tb] != letter && s[tb] != '.') || (s[tb + 4] != letter && s[tb + 4] != '.') ||
            (s[tb + 8] != letter && s[tb + 8] != '.') || (s[tb + 12] != letter && s[tb + 12] != '.');
}

auto is_deeper_in_burrow_free(const std::string &s, int index)
{
    while ((index += 4) < s.length())
        if (s[index] == '.')
            return true;

    return false;
}

auto is_path_free(const std::string &s, int a, int b)
{
    for (auto i : path[a][b])
        if (s[i] != '.')
            return false;
    return true;
}

auto do_part(std::string input, bool part2)
{
    struct State
    {
        int cost, projected;
        std::string s;

        bool operator<(const State &other) const
        {
            if (projected > other.projected) return true;
            if (projected < other.projected) return false;
            if (cost > other.cost) return true;
            if (cost < other.cost) return false;
            return s > other.s;
        }
    };

    std::priority_queue<State, std::vector<State>> q;
    std::set<std::pair<std::string, int>> seen;
    auto min_cost = INT_MAX;

    std::string start = part2 ? input.insert(11, "DCBADBAC") : input;
    std::string end = part2 ? ".......ABCDABCDABCDABCD" : ".......ABCDABCD";

    q.push({ 0, compute_projected_cost(start, end), start });

    while (!q.empty())
    {
        State state = q.top();
        q.pop();

        if (state.projected >= min_cost)
            continue;

        auto pos_cost = make_pair(state.s, state.cost);
        if (seen.find(pos_cost) != seen.end())
            continue;

        seen.insert(pos_cost);

        if (state.s == end)
        {
            min_cost = std::min(min_cost, state.cost);
            continue; 
        }

        for (auto from = 0; from <= end.length(); from++)
        {
            if (!isalpha(state.s[from])) continue;

            // don't move out of a finished burrow
            if (!burrow_has_wrong_letters(state.s, state.s[from]) && state.s[from] == end[from])
                continue;

            for (auto to = 0; to <= end.length(); to++)
            {
                if (state.s[to] != '.') continue;

                auto d = dist[from][to];
                if (d <= 0) continue;

                // Check if way is blocked by intermediate pieces
                if (!is_path_free(state.s, from, to))
                    continue;

                if (to > 6)
                {
                    // don't move into wrong burrow
                    if (state.s[from] != end[to])
                        continue;

                    // don't move into any burrow that still has wrong letters
                    if (burrow_has_wrong_letters(state.s, end[to]))
                        continue;

                    // when entering a burrow, only go to deepest available
                    if (is_deeper_in_burrow_free(state.s, to))
                        continue;
                }

                State new_state = state;
                std::swap(new_state.s[from], new_state.s[to]);
                new_state.cost = state.cost + d * letter_cost(state.s[from]);
                new_state.projected = new_state.cost + compute_projected_cost(new_state.s, end);
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
