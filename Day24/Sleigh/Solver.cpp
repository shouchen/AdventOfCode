// Sleigh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include "Solver.h"

unsigned Solver::s_numGroups = 3U;
std::vector<Solver::Package> Solver::s_packages;
Solver::Group Solver::s_groups[MAX_GROUPS + 1];
unsigned Solver::s_weightPerGroup;
std::vector<std::vector<Solver::Package>> Solver::s_solutions;
unsigned Solver::s_minSizeGroupOne = UINT_MAX;

unsigned long long Solver::Solve(std::vector<unsigned> weights, unsigned numGroups)
{
    // Better thing to do here is a reverse sort
    std::reverse(weights.begin(), weights.end());

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

unsigned Solver::GetWeight(unsigned group)
{
    auto weight = 0U;
    for (auto p : s_packages)
        if (p.group == group)
            weight += p.weight;
    return weight;
}

unsigned long long Solver::GetQE(const std::vector<Package> &pkgs)
{
    auto qe = 1ULL;
    for (auto p : pkgs)
        if (p.group == 1)
            qe *= p.weight;
    return qe;
}

void Solver::AssignPackage(Package &pkg, unsigned group)
{
    assert(pkg.group != group);

    s_groups[pkg.group].weight -= pkg.weight;
    s_groups[pkg.group].count--;
    pkg.group = group;
    s_groups[pkg.group].weight += pkg.weight;
    s_groups[pkg.group].count++;
}

void Solver::AddSolution(const std::vector<Package> &pkgs)
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

bool Solver::CheckWayForGroupN(unsigned n, std::vector<Package>::iterator from, std::vector<Package>::iterator to, unsigned minPackages)
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

void Solver::PopulateGroup1Recurse(std::vector<Package>::iterator from, std::vector<Package>::iterator to)
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

unsigned long long Solver::GetLowestQe()
{
    auto minQe = ULLONG_MAX;
    for (auto &s : s_solutions)
    {
        unsigned long long qe = GetQE(s);
        minQe = std::min(minQe, qe);
    }

    return minQe;
}
