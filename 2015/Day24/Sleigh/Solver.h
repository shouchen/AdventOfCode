// Sleigh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <algorithm>

class Solver
{
public:
    static unsigned long long Solve(std::vector<unsigned> weights, unsigned numGroups);

private:
    struct Package
    {
        unsigned weight, group;
    };

    struct Group
    {
        unsigned weight, count;
    };

    static void PopulateGroups(std::vector<Package>::iterator from, std::vector<Package>::iterator to);
    static bool CheckWayForGroupNAndBeyond(unsigned n, std::vector<Package>::iterator from, std::vector<Package>::iterator to);
    static void ProcessSolution();
    static void AssignPackage(Package &pkg, unsigned group);

    static unsigned s_numGroups;
    static unsigned s_weightPerGroup;
    static unsigned s_lowestGroupOneSize;
    static unsigned long long s_lowestGroupOneQeForLowestSize;

    static std::vector<Package> s_packages;
    static Group *s_groups;
};
