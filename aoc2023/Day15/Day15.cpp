#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <ctype.h>
#include <algorithm>
#include <numeric>
#include <regex>
#include <cassert>

using namespace std;

struct Lens
{
    string label = "";
    char focus = '0';
};

vector<vector<Lens>> boxes;

void dump()
{
    for (int i = 0; i < boxes.size(); i++)
    {
        if (boxes[i].size() == 0)
            continue;

        cout << "Box " << i << ": ";

        for (int j = 0; j < boxes[i].size(); j++)
            cout << " [" << boxes[i][j].label << " " << boxes[i][j].focus << "]";
        cout << endl;
    }
    cout << endl;
}

int ahash(const std::string &s)
{
    int curr = 0;

    for (int i = 0; i < s.length(); i++)
    {
        char c = s[i];
        curr += c;
        curr *= 17;
        curr = curr % 256;
    }

    return curr;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    auto c = ' ';
    int total = 0;
    string s;

    while (file >> c)
    {
        if (c == ',')
        {
            total += ahash(s);
            s.clear();
            continue;
        }

        s.push_back(c);
    }

    total += ahash(s);
    return total;
}

auto do_part2(const std::string &filename)
{
    boxes.clear();
    for (int i = 0; i < 256; i++)
        boxes.push_back(vector<Lens>());

    std::ifstream file(filename);
    char c = ' ', dash_equal = '-', digit = ' ', comma = ',';

    while (file >> c)
    {
        string label(1, c);
        for (;;)
        {
            file >> c;
            if (c == '-' || c == '=')
            {
                dash_equal = c;
                break;
            }
            label.push_back(c);
        }

        auto &box = boxes[ahash(label)];

        if (dash_equal == '-')
        {
            int j = 0;
            for (; j < box.size(); j++)
                if (box[j].label == label)
                    break;

            if (j < box.size())
            {
                for (; j < box.size() - 1; j++)
                    box[j] = box[j + 1];

                box.pop_back();
            }
        }
        else if (dash_equal == '=')
        {
            file >> digit;

            int j = 0;
            for (j = 0; j < box.size(); j++)
            {
                if (box[j].label == label)
                    break;
            }

            if (j < box.size())
                box[j].focus = digit;
            else
                box.push_back(Lens{ label, digit });
        }
        else
            assert(false);

        //cout << "After \"" << label << dash_equal << digit << "\"" << endl;
        //dump();

        file >> dash_equal; //comma
    }

    //dump();

    long long total = 0;
    for (int i = 0; i < boxes.size(); i++)
    {
        for (int j = 0; j < boxes[i].size(); j++)
        {
            total += (i + 1) * (j + 1) * (boxes[i][j].focus - '0');
        }
    }

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
