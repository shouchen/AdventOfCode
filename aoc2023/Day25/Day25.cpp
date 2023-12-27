#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <random>
#include <cassert>

// This is a graph "min-cut" problem.
// https://en.wikipedia.org/wiki/Minimum_cut
//
// Stoer-Wagner has a polynomial-time solution for weighted graphs:
// https://en.wikipedia.org/wiki/Stoer%E2%80%93Wagner_algorithm
//
// But this one is not weighted, so it's possible to just use
// https://en.wikipedia.org/wiki/Karger%27s_algorithm
//
// which is more efficient. This one also involves a small but non-zero chance
// of failing to find the minimal cut on any particular run, but it is fast,
// so we can just run it repeatedly until the known mincut of size 3 is found.

struct DSU
{
    int n;
    std::vector<int> par;

    DSU(int n)
    {
        this->n = n;
        par.resize(n + 1, -1);
    }

    int Find(int i)
    {
        return par[i] < 0 ? i : par[i] = Find(par[i]);
    }

    // returns true if a and b were not connected
    auto Union(int a, int b)
    {
        a = Find(a), b = Find(b);
        if (a == b) return false;

        if (par[a] > par[b]) std::swap(a, b);
        par[a] += par[b];
        par[b] = a;
        return true;
    }
};

std::vector<std::vector<std::pair<int, int>>> adj;
std::vector<std::pair<int, int>> edges;
auto V = 0, E = 0;
auto rng = std::mt19937(unsigned(time(nullptr)));

void read_input(const std::string &filename)
{
    std::ifstream file("input.txt");
//    adj.resize(1500); // must be large enough for all vertices (which is more than input lines)

    std::string line, s, t;
    std::map<std::string, int> name_to_index;

    while (getline(file, line))
    {
        std::stringstream ss(line);

        ss >> s;
        s.pop_back();
        auto s_index = name_to_index.count(s) ? name_to_index[s] : (name_to_index[s] = V++);

        while (ss >> t)
        {
            auto t_index = name_to_index.count(t) ? name_to_index[t] : (name_to_index[t] = V++);
            while (adj.size() < name_to_index.size())
                adj.push_back(std::vector<std::pair<int, int>>());

            adj[s_index].push_back({ t_index, int(edges.size()) });
            adj[t_index].push_back({ s_index, int(edges.size()) });
            edges.push_back({ s_index, t_index });
        }
    }

    E = int(edges.size());
}

auto do_karger()
{
    DSU dsu(V);

    auto distinct = [&](int e) {
        return dsu.Find(edges[e].first) != dsu.Find(edges[e].second);
    };

    auto getE = [&]() -> int {
        std::vector<int> v;
        for (auto i = 0; i < E; i++)
            if (distinct(i))
                v.push_back(i);

        return v[rng() % int(v.size())];
    };

    for (auto n = V; n > 2; --n)
    {
        auto temp = edges[getE()];
        dsu.Union(temp.first, temp.second);
    }

    std::vector<int> ans;
    for (auto i = 0; i < E; i++)
        if (distinct(i))
            ans.push_back(i);

    return ans;
}

auto find_size_three_mincut()
{
    for (;;)
    {
        auto v = do_karger();
        if (v.size() == 3)
            return v;
    }
}

auto get_product_of_two_group_sizes(std::vector<int> &removed_edges)
{
    auto seen = std::vector<bool>(V);
    auto group_one_size = 0;

    auto dfs = [&](auto &self, int i) -> void {
        if (seen[i])
            return;

        seen[i] = true;
        group_one_size++;

        for (auto a : adj[i])
        {
            auto j = a.first, e = a.second;
            if (e != removed_edges[0] && e != removed_edges[1] && e != removed_edges[2])
                self(self, j);
        }
    };

    dfs(dfs, 0);
    return group_one_size * (V - group_one_size);
}

int main()
{
    read_input("input.txt");
    auto removed_edges = find_size_three_mincut();
    auto part1 = get_product_of_two_group_sizes(removed_edges);

    std::cout << "Part One: " << part1 << std::endl;

    assert(part1 == 547080);
    return 0;
}