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

    static unsigned long long Solve(std::vector<unsigned> weights, unsigned numGroups)
    {
        // Set up globals and reset any state
        s_numGroups = numGroups;
        s_solutions.clear();

        // Default everything to the last group
        s_packages.clear();
        for (auto weight : weights)
            s_packages.push_back(Solver::Package{ weight, numGroups });

        for (int i = 0; i < sizeof(s_groups) / sizeof(s_groups[0]); i++)
        {
            s_groups[i].count = 0;
            s_groups[i].weight = 0;
        }

        for (auto p : Solver::s_packages)
            s_groups[s_numGroups].weight += p.weight;
        s_weightPerGroup = s_groups[s_numGroups].weight / s_numGroups;

        s_numGroups = numGroups;
        s_minSizeGroupOne = UINT_MAX;
        PopulateGroup1Recurse(s_packages.begin(), s_packages.end());

        return GetLowestQe();
    }

private:
    struct Package
    {
        unsigned weight, group;
    };

    struct Group
    {
        unsigned weight, count;
    };

    static unsigned GetWeight(unsigned group)
    {
        auto weight = 0U;
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
    unsigned weight;
    std::vector<unsigned> weights;

    std::ifstream f("Input.txt");
    while (f >> weight)
        weights.push_back(weight);

    // TODO: This should really be a reverse sort, and the solver should do it
    std::reverse(weights.begin(), weights.end());

    auto qe = Solver::Solve(weights, 3);
    assert(qe == 11266889531);
    std::cout << "part one: " << qe << std::endl;

    qe = Solver::Solve(weights, 4);
    assert(qe == 77387711);
    std::cout << "part two: " << qe << std::endl;
}
