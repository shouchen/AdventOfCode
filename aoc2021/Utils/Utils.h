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
#include <limits>
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

// DebuggingUtils
std::ostream &dump_grid(ostream& o, const std::vector<std::vector<int>> &grid);

template<typename T>
void determine_sparse_grid_extent(const std::map <std::pair<int, int>, T> &grid, int &min_first, int &max_first, int &min_second, int &max_second)
{
    min_first = INT_MAX, max_first = INT_MIN, min_second = INT_MAX, max_second = INT_MIN;
    for (const auto &i : grid)
    {
        if (i.first.first < min_first) min_first = i.first.first;
        if (i.first.first > max_first) max_first = i.first.first;
        if (i.first.second < min_second) min_second = i.first.second;
        if (i.first.second > max_second) max_second = i.first.second;
    }
}

template<typename T>
auto count_cells_in_sparse_grid(const std::map <std::pair<int, int>, T>& grid)
{
    auto count = 0;
    for (const auto &i : grid)
    {
        //if (i.second > 1) // replace this with some other constraint
            count++;
    }

    return count;
}
