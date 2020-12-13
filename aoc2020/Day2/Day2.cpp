#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>

auto process_input(const std::string &filename)
{
    std::ifstream file(filename);
    auto a = 0UL, b = 0UL;
    auto c = ' ', dash = '-', colon = ':';
    std::string password;

    auto retval = std::make_pair(0, 0);
    while (file >> a >> dash >> b >> c >> colon >> password)
    {
        auto n = std::count(password.begin(), password.end(), c);

        if (n >= a && n <= b)
            retval.first++;

        if ((password[a - 1] == c) != (password[b - 1] == c))
            retval.second++;
    }

    return retval;
}

int main()
{
    auto result = process_input("input.txt");
    std::cout << "Part One: " << result.first << std::endl;
    std::cout << "Part Two: " << result.second << std::endl;

    assert(result.first == 519);
    assert(result.second == 708);
    return 0;
}
