#include "stdafx.h"
#include "Solver.h"
#include <algorithm>

unsigned Solver::minLength;
std::map<unsigned, unsigned> Solver::lengthToCount;

void Solver::Solve(const std::vector<unsigned> &containers, unsigned amount, unsigned &numCombinations, unsigned &numOfMinimumLength)
{
    minLength = UINT_MAX;
    lengthToCount.empty();

    numCombinations = Solve(containers.begin(), containers.end(), amount);
    numOfMinimumLength = lengthToCount[minLength];
}

unsigned Solver::Solve(std::vector<unsigned>::const_iterator begin, std::vector<unsigned>::const_iterator end, unsigned amount, unsigned numChosenSoFar)
{
    unsigned numCombinations = 0;

    if (begin != end)
    {
        auto curr = *begin++;

        if (amount == curr)
        {
            // Record a solution!
            lengthToCount[++numChosenSoFar]++;
            minLength = std::min(minLength, numChosenSoFar);

            numCombinations = 1;
        }
        else if (amount > curr)
        {
            // Enough remains to select current or not (must sum both paths)
            numCombinations = Solve(begin, end, amount - curr, numChosenSoFar + 1);
        }

        // To whatever happened above, add combinations for items beyond this one.
        numCombinations += Solve(begin, end, amount, numChosenSoFar);
    }

    return numCombinations;
}
