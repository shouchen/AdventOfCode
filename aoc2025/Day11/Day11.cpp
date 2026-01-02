#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <cassert>

struct Node
{
    std::string name;
    std::vector<Node *> adjacents;
};

auto graph = std::map<std::string, Node *>();

auto read_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string token;
    Node *from = nullptr, *to = nullptr;
    auto retval = 0;

    while (file >> token)
    {
        if (token.back() == ':')
        {
            token.pop_back();
            if (graph.find(token) == graph.end())
                graph[token] = from = new Node{ token };
            else
                from = graph[token];
        }
        else
        {
            if (graph.find(token) == graph.end())
                graph[token] = to = new Node{ token };
            else
                to = graph[token];

            if (from != nullptr)
                from->adjacents.push_back(to);
        }
    }

    return retval;
}

std::set<Node *> visited;
std::map<std::pair<Node *, Node *>, unsigned long long> memo;

auto recur(Node *curr, Node *dest)
{
    if (curr == dest)
        return (visited.find(graph["dac"]) != visited.end() && visited.find(graph["fft"]) != visited.end()) ? 1ULL : 1ULL;

    auto temp = memo.find({ curr, dest });
    if (temp != memo.end())
        return temp->second;

    if (visited.find(curr) != visited.end())
        return 0ULL;

    visited.insert(curr);

    auto retval = 0ULL;
    for (auto next : curr->adjacents)
        retval += recur(next, dest);

    visited.erase(curr);

    memo.insert({ { curr, dest }, retval });
    return retval;
}

auto do_part1()
{
    Node *you = graph["you"];
    Node *out = graph["out"];

    return recur(you, out);
}

auto do_part2()
{
    Node *svr = graph["svr"];
    Node *out = graph["out"];
    Node *dac = graph["dac"];
    Node *fft = graph["fft"];

    auto fft_dac = recur(fft, dac);
    auto dac_fft = recur(dac, fft);

    if (fft_dac)
    {
        auto svr_fft = recur(svr, fft);
        auto dac_out = recur(dac, out);

        return svr_fft * fft_dac * dac_out;
    }
    else
    {
        auto svr_dac = recur(svr, dac);
        auto fft_out = recur(fft, out);

        return svr_dac * dac_fft * fft_out;
    }
}

int main()
{
    read_data("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 753);

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 450854305019580);
    return 0;
}
