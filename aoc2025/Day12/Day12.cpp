#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#include <cassert>

using Shape = std::vector<std::string>;
using Shapes = std::vector<Shape>;

Shapes shapes;

auto get_shape_area(const Shape &shape)
{
    auto retval = 0U;
    for (int i = 0; i < shape.size(); i++)
        for (int j = 0; j < shape[i].size(); j++)
            if (shape[i][j] == '#')
                retval++;
    return retval;
}

auto will_fit(int width, int length, const std::vector<int> &quantities)
{
    auto packages_require = 0U;
    for (auto i = 0; i < shapes.size(); i++)
        packages_require += quantities[i] * get_shape_area(shapes[i]);

    // Disquality if target area is too small, even with perfect interlocking
    if (packages_require > width * length)
        return false;

    // Auto-succeed if the number of 3x3's in target area covers all the pieces, no interlocking required
    auto num_packages = 0U;
    for (auto &q : quantities)
        num_packages += q;

    if (num_packages <= (width / 3) * (length / 3)) // TODO: Shouldn't hard-code 3
        return true;

    // Case three = have to interlock - does this even hit?
    assert(false);
    return false;
}

auto do_part(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto comma = ',', parenth = '(', brace = '{', token = ' ';
    auto n = 0, num_indicators = 0;

    auto buttons = std::vector<unsigned int>{};
    auto joltages = std::vector<unsigned int>{};
    auto goal = 0U;
    unsigned retval = 0U;


    while (std::getline(file, line))
    {
        auto x = line.find('x');
        auto colon = line.find(':');

        if (x == std::string::npos) // shape definition
        {
            Shape shape;

            while (std::getline(file, line))
            {
                if (line.empty())
                    break;

                shape.push_back(line);
            }

            shapes.push_back(shape);
        }
        else // space under tree
        {
            auto width = atoi(line.substr(0, x).c_str());
            auto length = atoi(line.substr(x + 1, colon - x - 1).c_str());
            auto rest = line.substr(colon + 1);
            auto n = 0;
            std::vector<int> quantities;

            auto ss = std::stringstream(rest);
            while (ss >> n)
                quantities.push_back(n);

            if (will_fit(width, length, quantities))
                retval++;
        }
    }

    return retval;
}

int main()
{
    auto part1 = do_part("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 479);

    std::cout << "Part Two: N/A" << std::endl;
    return 0;
}
