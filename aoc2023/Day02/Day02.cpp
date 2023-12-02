#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto retval = std::make_pair(0, 0);

    while (std::getline(file, line))
    {
        std::istringstream is(line);
        std::string game, color;
        auto g = 0, n = 0, red = 0, green = 0, blue = 0;
        auto colon = ':';
        bool possible = true;

        is >> game >> g >> colon;

        while (is >> n >> color)
        {
            if (color.back() == ',' || color.back() == ';')
                color.pop_back();

            if (color == "red")
            {
                if (n > 12) possible = false;
                red = std::max(red, n);
            }
            else if (color == "green")
            {
                if (n > 13) possible = false;
                green = std::max(green, n);
            }
            else if (color == "blue")
            {
                if (n > 14) possible = false;
                blue = std::max(blue, n);
            }
        }

        if (possible)
            retval.first += g;

        retval.second += red * green * blue;
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 2679);
    assert(answer.second == 77607);
    return 0;
}
