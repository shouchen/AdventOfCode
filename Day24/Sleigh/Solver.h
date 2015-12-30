// Sleigh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <algorithm>

#define MAX_GROUPS 4

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

    static unsigned GetWeight(unsigned group);
    static unsigned long long GetQE(const std::vector<Package> &pkgs);
    static void AssignPackage(Package &pkg, unsigned group);
    static void AddSolution(const std::vector<Package> &pkgs);
    static bool CheckWayForGroupN(unsigned n, std::vector<Package>::iterator from, std::vector<Package>::iterator to, unsigned minPackages);
    static void PopulateGroup1Recurse(std::vector<Package>::iterator from, std::vector<Package>::iterator to);
    static unsigned long long GetLowestQe();

    static unsigned s_numGroups;
    static std::vector<Package> s_packages;
    static Group s_groups[MAX_GROUPS + 1];
    static unsigned s_weightPerGroup;
    static std::vector<std::vector<Package>> s_solutions;
    static unsigned s_minSizeGroupOne;
};
