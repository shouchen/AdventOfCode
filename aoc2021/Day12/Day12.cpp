#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cassert>

struct Cave
{
    enum Type { Big, Small, Start, End };

    Cave(const std::string &name)
    {
        if (name == "start") type = Start;
        else if (name == "end") type = End;
        else type = islower(name[0]) ? Small : Big;
    }

    Type type;
    std::vector<const Cave *> adjacent;
};

std::set<const Cave *> visited;
const Cave *visited_twice;
auto count = 0;

void count_routes(const Cave *cave, bool part2)
{
    if (cave->type == Cave::End)
    {
        count++;
        return;
    }

    if (cave->type == Cave::Small)
        if (visited.find(cave) == visited.end())
            visited.insert(cave);
        else if (part2 && !visited_twice)
            visited_twice = cave;
        else
            return;

    for (auto c : cave->adjacent)
        if (c->type != Cave::Start)
            count_routes(c, part2);

    if (cave->type == Cave::Small)
        if (visited_twice == cave)
            visited_twice = nullptr;
        else
            visited.erase(cave);
}

Cave *read_input(const std::string &filename)
{
    std::map<std::string, Cave *> cave_dict;
    std::ifstream file(filename);
    std::string a, b;

    while (getline(file, a, '-') && getline(file, b))
    {
        if (cave_dict.find(a) == cave_dict.end())
            cave_dict[a] = new Cave(a);
        if (cave_dict.find(b) == cave_dict.end())
            cave_dict[b] = new Cave(b);

        cave_dict[a]->adjacent.push_back(cave_dict[b]);
        cave_dict[b]->adjacent.push_back(cave_dict[a]);
    }

    return cave_dict["start"];
}

auto do_part(Cave *start, bool part2)
{
    count = 0;
    count_routes(start, part2);
    return count;
}

int main()
{
    auto start = read_input("input.txt");

    auto part1 = do_part(start, false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part(start, true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 5457);
    assert(part2 == 128506);
    return 0;
}
