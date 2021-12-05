#include "Utils.h"

std::vector<std::vector<int>> read_matrix(std::ifstream &file)
{
    std::vector<std::vector<int>> retval;

    auto n = 0;
    std::string line;

    while (getline(file, line) && !line.empty())
    {
        retval.push_back(std::vector<int>());
        
        std::istrstream ss(line.c_str());
        auto n = 0;

        while (ss >> n)
            retval.back().push_back(n);
    }

    return retval;
}
