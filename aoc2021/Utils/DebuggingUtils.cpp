#include <iomanip>
#include "Utils.h"

std::ostream &dump_grid(ostream &o, const std::vector<std::vector<int>> &grid)
{
    for (const auto& row : grid)
    {
        for (const auto& cell : row)
            o << setw(3) << cell;

        o << std::endl;
    }

    return o << std::endl;    
}
