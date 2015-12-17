#pragma once

#include <vector>
#include <map>

class Solver
{
public:
    static void Solve(const std::vector<unsigned> &containers, unsigned amount, unsigned &numCombinations, unsigned &numOfMinimumLength);

private:
    static unsigned Solve(std::vector<unsigned>::const_iterator begin, std::vector<unsigned>::const_iterator end, unsigned amount, unsigned numChosenSoFar = 0);

    static unsigned minLength;
    static std::map<unsigned, unsigned> lengthToCount;
};
