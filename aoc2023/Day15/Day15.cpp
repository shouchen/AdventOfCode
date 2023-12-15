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

auto solve(const std::string &filename)
{
    std::vector<std::vector<Lens>> boxes(256);
    std::ifstream file(filename);
    std::string s;
    auto retval = std::make_pair(0, 0);

    while (std::getline(file, s, ','))
    {
        retval.first += hash(s);

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
            if (lens == box.end())
                box.push_back(Lens{ label, focus });
            else
                lens->focus = focus;
        else
            if (lens != box.end())
                box.erase(lens);
    }

    for (auto box = 0; box < boxes.size(); box++)
        for (auto lens = 0; lens < boxes[box].size(); lens++)
            retval.second += (box + 1) * (lens + 1) * boxes[box][lens].focus;

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 511498);
    assert(answer.second == 284674);
    return 0;
}
