#include <iostream>
#include <fstream>
#include <strstream>
#include <vector>
#include <string>
#include <array>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <regex>
#include <cassert>

using namespace std;

// ParsingUtils
std::vector<int> parse_string_into_ints_by_commas(const std::string &s);

// ReadingUtils
std::vector<std::vector<int>> read_matrix(std::ifstream& file);

template<typename T>
std::vector<T> read_lines_into_vector(std::ifstream& file)
{
    std::vector<T> retval;
    int x;

    while (file >> x)
        retval.push_back(x);

    return retval;
}
