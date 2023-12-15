#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

struct Lens
{
    std::string label = "";
    int focus = 0;
};

auto ahash(const std::string &s)
{
    auto curr = 0;

    for (auto c : s)
        curr = (curr + c) * 17 % 256;

    return curr;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    auto c = ' ';
    auto total = 0;

    while (file >> c)
    {
        std::string s(1, c);
        while (file >> c && (c != ','))
            s.push_back(c);

        total += ahash(s);
    }

    return total;
}

auto do_part2(const std::string &filename)
{
    std::vector<std::vector<Lens>> boxes(256);
    std::ifstream file(filename);
    auto c = ' ', digit = ' ', comma = ',';

    while (file >> c)
    {
        std::string label(1, c);
        while (file >> c && (c != '-' && c != '='))
            label.push_back(c);

        auto &box = boxes[ahash(label)];
        auto found = std::find_if(box.begin(), box.end(),
            [&label](auto &x) { return x.label == label; });

        if (c == '-')
        {
            if (found != box.end())
                box.erase(found);
        }
        else
        {
            file >> digit;

            if (found != box.end())
                found->focus = digit - '0';
            else
                box.push_back(Lens{ label, digit - '0'});
        }

        file >> comma;
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
