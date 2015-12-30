// Sleigh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

#define MAX_GROUPS 4

class Solver
{
public:
    struct Package
    {
        unsigned weight;
        unsigned group;
    };

    struct Group
    {
        unsigned weight;
        unsigned count;
    };

    static unsigned GetWeight(unsigned group)
    {
        unsigned weight = 0;
        for (auto p : s_packages)
        if (p.group == group)
            weight += p.weight;
        return weight;
    }

    static unsigned long long GetQE(const std::vector<Package> &pkgs)
    {
        auto qe = 1ULL;
        for (auto p : pkgs)
        if (p.group == 1)
            qe *= p.weight;
        return qe;
    }

    static void AssignPackage(Package &pkg, unsigned group)
    {
        assert(pkg.group != group);

        s_groups[pkg.group].weight -= pkg.weight;
        s_groups[pkg.group].count--;
        pkg.group = group;
        s_groups[pkg.group].weight += pkg.weight;
        s_groups[pkg.group].count++;
    }

    static void AddSolution(const std::vector<Package> &pkgs)
    {
        unsigned size = s_groups[1].count;
        if (size > s_minSizeGroupOne) return;

        if (size < s_minSizeGroupOne)
        {
            s_minSizeGroupOne = size;
            s_solutions.clear();
        }

        s_solutions.push_back(pkgs);
    }

    static bool CheckWayForGroupN(unsigned n, std::vector<Package>::iterator from, std::vector<Package>::iterator to, unsigned minPackages)
    {
        if (n == s_numGroups)
        {
            bool retval = s_groups[s_numGroups].count >= minPackages;
            if (retval)
                AddSolution(s_packages);
            return retval;
        }

        if (from == to || s_groups[n].weight > s_weightPerGroup) return false;

        if (s_groups[n].weight == s_weightPerGroup)
        {
            if (s_groups[n].count < minPackages) return false;

            return CheckWayForGroupN(n + 1, s_packages.begin(), s_packages.end(), minPackages);
        }

        for (auto curr = from; curr != to; curr++)
        {
            if (curr->group == s_numGroups)
            {
                AssignPackage(*curr, n);
                bool retval = CheckWayForGroupN(n, curr + 1, to, minPackages);
                AssignPackage(*curr, s_numGroups);

                if (retval) return true;
            }
        }

        return false;
    }

    static void PopulateGroup1Recurse(std::vector<Package>::iterator from, std::vector<Package>::iterator to)
    {
        if (s_groups[1].weight > s_weightPerGroup) return;

        if (s_groups[1].weight == s_weightPerGroup)
        {
            CheckWayForGroupN(2, s_packages.begin(), s_packages.end(), s_groups[1].count);
            return;
        }

        if (s_groups[1].count > s_minSizeGroupOne) return;

        for (auto curr = from; curr != to; curr++)
        {
            AssignPackage(*curr, 1);
            PopulateGroup1Recurse(curr + 1, to);
            AssignPackage(*curr, s_numGroups);
        }
    }

    static unsigned long long GetLowestQe()
    {
        auto minQe = ULLONG_MAX;
        for (auto &s : s_solutions)
        {
            unsigned long long qe = GetQE(s);
            minQe = std::min(minQe, qe);
        }

        return minQe;
    }

    static unsigned s_numGroups;
    static std::vector<Package> s_packages;
    static Group s_groups[MAX_GROUPS + 1];
    static unsigned s_weightPerGroup;
    static std::vector<std::vector<Package>> s_solutions;
    static unsigned s_minSizeGroupOne;
};

unsigned Solver::s_numGroups = 3U;
std::vector<Solver::Package> Solver::s_packages;
Solver::Group Solver::s_groups[MAX_GROUPS + 1];
unsigned Solver::s_weightPerGroup;
std::vector<std::vector<Solver::Package>> Solver::s_solutions;
unsigned Solver::s_minSizeGroupOne = UINT_MAX;


void _tmain(int argc, _TCHAR *argv[])
{
    std::ifstream f("Input.txt");
    unsigned weight;
    while (f >> weight)
        Solver::s_packages.push_back(Solver::Package{ weight, Solver::s_numGroups });

    // TODO: This should really be a reverse sort
    std::reverse(Solver::s_packages.begin(), Solver::s_packages.end());

    // Everything starts in last group by default
    for (auto p : Solver::s_packages)
        Solver::s_groups[Solver::s_numGroups].weight += p.weight;
    Solver::s_weightPerGroup = Solver::s_groups[Solver::s_numGroups].weight / Solver::s_numGroups;

    Solver::s_numGroups = 3;
    Solver::s_minSizeGroupOne = UINT_MAX;
    Solver::PopulateGroup1Recurse(Solver::s_packages.begin(), Solver::s_packages.end());

    auto qe = Solver::GetLowestQe();
    assert(qe == 11266889531);
    std::cout << "part one: " << qe << std::endl;

    //s_numGroups = 4;
    //Solver::s_minSizeGroupOne = UINT_MAX;
    //PopulateGroup1Recurse(packages.begin(), packages.end());

    //qe = GetLowestQe();
    //assert(qe == 77387711);
    //cout << "part two: " << qe << endl;
}
