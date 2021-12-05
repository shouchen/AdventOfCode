#include <strstream>
#include "Utils.h"


std::vector<int> parse_string_into_ints_by_commas(const std::string &s)
{
    std::vector<int> retval;

    std::istrstream ss(s.c_str());
    auto n = 0;
    auto comma = ','; // any non-numeric, non-whitespace

    while (ss >> n)
    {
        retval.push_back(n);
        ss >> comma;
    }

    return retval;
}

// Other:
//
// regex:
// std::smatch sm;
// if (std::regex_match(string, sm, std::regex("^(\\d*)in$")))
