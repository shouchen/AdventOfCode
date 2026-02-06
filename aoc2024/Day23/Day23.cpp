#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <string>
#include <algorithm>
#include <cassert>

using ComputerSet = std::set<std::string>;
using ComputerUSet = std::unordered_set<std::string>;
using AdjacencyMap = std::unordered_map<std::string, ComputerUSet>;

inline auto includes_computer_starting_with_t(const ComputerSet &set)
{
    auto retval = false;
    for (auto &computer : set)
        if (computer.front() == 't')
            return true;

    return false;
}

inline auto contains_all(const ComputerSet &s1, const ComputerUSet &s2)
{
    for (const auto &elem : s1)
        if (!s2.contains(elem))
            return false;

    return true;
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    AdjacencyMap map;
    std::pair<int, std::string> retval;

    // read the graph
    while (std::getline(file, line))
    {
        auto c1 = line.substr(0, 2), c2 = line.substr(3);
        map[c1].insert(c2); map[c2].insert(c1);
    }

    // build sets of size 3 ("triangles") - needed for both parts
    std::set<ComputerSet> triangles;
    for (const auto &[c1, c2s] : map)
        for (const auto &c2 : c2s)
            for (const auto c3 : map[c2])
                if (c1 != c3 && map[c3].contains(c1))
                    triangles.insert({ c1, c2, c3 });

    // compute part 1 result
    for (auto &triangle : triangles)
        if (includes_computer_starting_with_t(triangle))
            retval.first++;

    // create supersets by growing the triangles
    std::set<ComputerSet> current = triangles;

    for (;;)
    {
        std::set<ComputerSet> next;

        for (const auto &s : current)
            for (const auto &[comp, connections] : map)
                if (connections.size() >= s.size() && contains_all(s, connections))
                {
                    auto superset = s;
                    superset.insert(comp);
                    next.insert(superset);
                }

        if (next.empty()) break;
        current = next;
    }

    // format the part 2 result
    auto lan_party = *current.begin();
    std::ostringstream oss;
    std::string separator;

    for (auto &elem : lan_party)
    {
        oss << separator << elem;
        separator = ",";
    }

    retval.second = oss.str();
    return retval;
}

int main(int argc, char *argv[])
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 1064);
    assert(answer.second == "aq,cc,ea,gc,jo,od,pa,rg,rv,ub,ul,vr,yy");
    return 0;
}