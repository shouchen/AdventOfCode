#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <regex>
#include <algorithm>
#include <cassert>

using namespace std;

void read_formatted_file_line(const std::string& filename)
{
    std::ifstream file(filename);
    std::string s;
    auto n = 0;

    while (file >> s >> n)
    {
        std::cout << s << " " << n;
    }
}

void read_file_lines_in_groups_separated_by_blanks_lines(const std::string& filename)
{
    std::ifstream file(filename);
    while (!file.eof())
    {
        std::string s;
        while (std::getline(file, s) && !s.empty())
        {
            // process line inside current group
            std::cout << s;
        }

        // process above group as a whole
    }
}

void read_file_with_strstream(const std::string& filename)
{
    std::ifstream file(filename);
    std::string(s);

    while (file >> s)
    {
        std::stringstream ss(s);
        // read from the string
    }
}

void regex()
{
    std::string input;
    std::smatch sm;

    if (std::regex_match(input, sm, std::regex("^(\\d*)in$")))
    {
        // reading a number
        auto n = atoi(sm[1].str().c_str());
    }

    // std::regex("^#[0-9a-f]{6}$")) &&
    // std::regex("^(amb|blu|brn|gry|grn|hzl|oth)$")) &&
    // std::regex("^\\d{9}$"));
}

auto process_input(const std::string &filename)
{
    auto retval = make_pair(0, 0);
    return retval;
}

int main()
{
    auto result = process_input("input.txt");
    std::cout << "Part One: " << result.first << std::endl;
    std::cout << "Part Two: " << result.second << std::endl;

    //assert(result.first == );
    //assert(result.second == );
    return 0;
}
