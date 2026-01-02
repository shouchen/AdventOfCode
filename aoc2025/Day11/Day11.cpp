#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cassert>

struct Node
{
    std::vector<Node *> adjacents;
};

std::map<std::string, Node *> graph;
std::map<std::pair<Node *, Node *>, unsigned long long> memo;

auto read_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string token;
    Node *from = nullptr, *to = nullptr;

    while (file >> token)
    {
        if (token.back() == ':')
        {
            token.pop_back();

            from = graph.find(token) == graph.end()
                ? graph[token] = new Node()
                : graph[token];
        }
        else
        {
            to = graph.find(token) == graph.end()
                ? graph[token] = new Node()
                : graph[token];

            from->adjacents.push_back(to);
        }
    }
}

auto recur(Node *curr, Node *dest)
{
    if (curr == dest)
        return 1ULL;

    auto temp = memo.find({ curr, dest });
    if (temp != memo.end())
        return temp->second;

    auto retval = 0ULL;
    for (auto next : curr->adjacents)
        retval += recur(next, dest);

    memo.insert({ { curr, dest }, retval });
    return retval;
}

auto do_part1()
{
    return recur(graph["you"], graph["out"]);
}

auto do_part2()
{
    Node *svr = graph["svr"], *out = graph["out"];
    Node *dac = graph["dac"], *fft = graph["fft"];

    auto fft_dac = recur(fft, dac), dac_fft = recur(dac, fft);

    return fft_dac
        ? recur(svr, fft) * fft_dac * recur(dac, out)
        : recur(svr, dac) * dac_fft * recur(fft, out);
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
