#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

std::vector<std::vector<bool>> adjacency(26*26, std::vector<bool>(26*26, false));
std::vector<std::string> computers;

inline int make_index(const std::string &name)
{
    return (name[0] - 'a') * 26 + (name[1] - 'a');
}

auto mark_adjacent(const std::string &c1, const std::string &c2)
{
    auto ndx1 = make_index(c1), ndx2 = make_index(c2);
    adjacency[ndx1][ndx2] = adjacency[ndx2][ndx1] = true;
}

auto are_adjacent(const std::string &c1, const std::string &c2)
{
    auto ndx1 = make_index(c1), ndx2 = make_index(c2);
    return adjacency[ndx1][ndx2];
}

auto do_part1(const std::string &filename)
{
    auto num_computers = 0;

    std::ifstream file(filename);
    std::string line;

    while (file >> line)
    {
        auto c1 = line.substr(0, 2), c2 = line.substr(3);
        computers.push_back(c1);
        computers.push_back(c2);
        mark_adjacent(c1, c2);
    }

    // De-dupe computer names
    std::sort(computers.begin(), computers.end());
    computers.erase(std::unique(computers.begin(), computers.end()), computers.end());

    // Find 3-loops
    auto retval = 0;

    for (auto i = 0; i < computers.size(); i++)
    {
        auto c1 = computers[i];
        auto ndx1 = make_index(c1);

        for (auto j = i + 1; j < computers.size(); j++)
        {
            auto c2 = computers[j];
            auto ndx2 = make_index(c2);

            if (!adjacency[ndx1][ndx2])
                continue;

            for (auto k = j + 1; k < computers.size(); k++)
            {
                auto c3 = computers[k];
                auto ndx3 = make_index(c3);

                if (!adjacency[ndx1][ndx3] || !adjacency[ndx2][ndx3])
                    continue;

                //std::cout << c1 << '-' << c2 << '-' << c3 << std::endl;
                if (c1[0] == 't' || c2[0] == 't' || c3 [0] == 't')
                    retval++;
            }
        }
    }

    return retval;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
        ;

    return -1;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 1064);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
