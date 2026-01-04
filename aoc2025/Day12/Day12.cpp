#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

struct Shape
{
    std::vector<std::string> form;
    unsigned length, width;
    unsigned packed_area;
};

std::vector<Shape> shapes;
auto max_shape_width = 0U, max_shape_length = 0U;

auto will_fit(int area_width, int area_length, const std::vector<int> &quantities)
{
    auto total_packed_area = 0ULL, num_packages = 0ULL;

    for (auto i = 0; i < quantities.size(); i++)
    {
        total_packed_area += quantities[i] * shapes[i].packed_area;
        num_packages += quantities[i];
    }

    // If total packed area exceeds target area, no fit is possible.
    if (total_packed_area > area_width * area_length)
        return false;

    // If target area has enough blocks to hold all pieces without interlocking, they fit.
    if (num_packages <= (area_width / max_shape_width) * (area_length / max_shape_length))
        return true;

    // This path is for more complex cases where interlocking could be required. For the example
    // data, this assert does hit, but for the real puzzle data, this case is irrelevant. If this
    // were to be hit with the puzzle data, we would need to implement a very complex packing
    // algorithm for a general solution, or else examine the data for patterns and build a
    // strategy for just the particular cases that occur in the puzzle input. Seeing that it's
    // Christmastime, this is welcomed as a gift from our benevolent puzzlemaker.
    assert(false);
    return false;
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto retval = 0;

    while (std::getline(file, line))
    {
        if (line.find('x') == std::string::npos)
        {
            std::vector<std::string> form;
            auto packed_area = 0LL;

            while (std::getline(file, line) && !line.empty())
            {
                form.push_back(line);
                packed_area += std::count(line.begin(), line.end(), '#');
            }

            auto length = unsigned(form.size()), width = unsigned(form[0].size());
            shapes.push_back({ form, length, width, unsigned(packed_area) });

            max_shape_length = std::max(max_shape_length, length);
            max_shape_width = std::max(max_shape_width, width);
        }
        else
        {
            auto width = 0, length = 0, n = 0;
            auto x = 'x', colon = ':';
            std::vector<int> quantities;

            auto ss = std::stringstream(line);
            ss >> width >> x >> length >> colon;

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
    auto part1 = solve("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 479);

    std::cout << "Part Two: N/A" << std::endl;
    return 0;
}
