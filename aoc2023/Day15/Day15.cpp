#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <cassert>

struct Lens
{
    std::string label = "";
    int focus = 0;
};

auto hash(const std::string &s)
{
    return std::accumulate(s.begin(), s.end(), 0,
        [](int a, char b) { return (a + b) * 17 % 256; });
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    auto total = 0;

    std::string s;
    while (std::getline(file, s, ','))
        total += hash(s);

    return total;
}

auto do_part2(const std::string &filename)
{
    std::vector<std::vector<Lens>> boxes(256);
    std::ifstream file(filename);
    std::string s;

    while (std::getline(file, s, ','))
    {
        auto focus = 0;

        if (isdigit(s.back()))
        {
            focus = s.back() - '0';
            s.pop_back();
        }

        s.pop_back();
        std::string label = s;

        auto &box = boxes[hash(label)];
        auto lens = std::find_if(box.begin(), box.end(),
            [&label](auto &x) { return x.label == label; });

        if (focus)
            if (lens != box.end())
                lens->focus = focus;
            else
                box.push_back(Lens{ label, focus });
        else
            if (lens != box.end())
                box.erase(lens);
    }

    auto total = 0;
    for (auto i = 0; i < boxes.size(); i++)
        for (auto j = 0; j < boxes[i].size(); j++)
            total += (i + 1) * (j + 1) * (boxes[i][j].focus);

    return total;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 511498);
    assert(part2 == 284674);
    return 0;
}
