#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <cassert>

// The example was simple, and it's enough to keep a list of lit pixels since
// the rest are all dark. However, in the puzzle input, the first character of
// the algorithm is '#', so the first step results in an infinite number of lit
// pixels and a finite set of dark ones. Similarly, the last character of the
// algorithm is '.' so the second step brings the image back to infinitely many
// dark pixels with a set of lit ones. Need to handle both of these cases (the
// simple example and the flip-flopping case of the puzzle input).

struct Image
{
    char default_pixels;
    std::set<std::pair<int, int>> exceptions;
};

auto new_pixel(const Image &image, int row, int col, const std::string &algo)
{
    auto n = 0;
    for (auto i = -1; i <= 1; i++)
        for (auto j = -1; j <= 1; j++)
        {
            n <<= 1;
            if (image.exceptions.find(std::make_pair(row + i, col + j)) != image.exceptions.end())
                n |= 1;
        }

    return algo[(image.default_pixels == '.') ? n : (n ^ 0x1ff)];
}

void do_step(Image &image, std::string &algo)
{
    auto min_row = INT_MAX, max_row = INT_MIN, min_col = INT_MAX, max_col = INT_MIN;
    for (auto &i : image.exceptions)
    {
        min_row = std::min(min_row, i.first);
        max_row = std::max(max_row, i.first);
        min_col = std::min(min_col, i.second);
        max_col = std::max(max_col, i.second);
    }

    Image next_image;
    auto index = (image.default_pixels == '.') ? 0 : 511;
    next_image.default_pixels = (algo[index] == '.') ? '.' : '#';

    for (auto row = min_row - 2; row <= max_row + 2; row++)
        for (auto col = min_col - 2; col <= max_col + 2; col++)
            if (new_pixel(image, row, col, algo) != next_image.default_pixels)
                next_image.exceptions.insert(std::make_pair(row, col));

    image = next_image;
}

auto process_input(const std::string &filename)
{
    Image input_image{ '.' };
    std::ifstream file(filename);
    std::string algo, s;

    file >> algo;

    auto row = 0;
    while (file >> s)
    {
        for (auto col = 0; col < s.length(); col++)
            if (s[col] == '#')
                input_image.exceptions.insert(std::make_pair(row, col));
        row++;
    }

    for (auto i = 0; i < 2; i++)
        do_step(input_image, algo);

    auto part1 = input_image.exceptions.size();

    for (int i = 0; i < 48; i++)
        do_step(input_image, algo);

    return std::make_pair(part1, input_image.exceptions.size());
}

int main()
{
    auto result = process_input("input.txt");
    std::cout << "Part 1: " << result.first << std::endl;
    std::cout << "Part 2: " << result.second << std::endl;

    assert(result.first == 5349);
    assert(result.second == 15806);
    return 0;
}
