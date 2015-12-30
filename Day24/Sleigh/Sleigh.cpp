// Sleigh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

auto s_numGroups = 3U;
#define MAX_GROUPS 4

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

std::vector<Package> s_packages;
Group s_groups[MAX_GROUPS + 1];

unsigned s_weightPerGroup;
std::vector<std::vector<Package>> s_solutions;
unsigned s_minSizeGroupOne = UINT_MAX;

unsigned GetWeight(unsigned group)
{
    unsigned weight = 0;
    for (auto p : s_packages)
        if (p.group == group)
            weight += p.weight;
    return weight;
}

unsigned long long GetQE(const std::vector<Package> &pkgs)
{
    auto qe = 1ULL;
    for (auto p : pkgs)
        if (p.group == 1)
            qe *= p.weight;
    return qe;
}

void AssignPackage(Package &pkg, unsigned group)
{
    assert(pkg.group != group);

    s_groups[pkg.group].weight -= pkg.weight;
    s_groups[pkg.group].count--;
    pkg.group = group;
    s_groups[pkg.group].weight += pkg.weight;
    s_groups[pkg.group].count++;
}

void AddSolution(const std::vector<Package> &pkgs)
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

bool CheckWayForGroupN(unsigned n, std::vector<Package>::iterator from, std::vector<Package>::iterator to, unsigned minPackages)
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

void PopulateGroup1Recurse(std::vector<Package>::iterator from, std::vector<Package>::iterator to)
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

unsigned long long GetLowestQe()
{
    auto minQe = ULLONG_MAX;
    for (auto &s : s_solutions)
    {
        unsigned long long qe = GetQE(s);
        minQe = std::min(minQe, qe);
    }

    return minQe;
}

void _tmain(int argc, _TCHAR *argv[])
{
    std::ifstream f("Input.txt");
    unsigned weight;
    while (f >> weight)
        s_packages.push_back(Package{ weight, s_numGroups });

    // TODO: This should really be a reverse sort
    std::reverse(s_packages.begin(), s_packages.end());

    // Everything starts in last group by default
    for (auto p : s_packages)
        s_groups[s_numGroups].weight += p.weight;
    s_weightPerGroup = s_groups[s_numGroups].weight / s_numGroups;

    s_numGroups = 3;
    PopulateGroup1Recurse(s_packages.begin(), s_packages.end());

    auto qe = GetLowestQe();
    assert(qe == 11266889531);
    std::cout << "part one: " << qe << std::endl;

    //s_numGroups = 4;
    //PopulateGroup1Recurse(packages.begin(), packages.end());

    //qe = GetLowestQe();
    //assert(qe == 77387711);
    //cout << "part two: " << qe << endl;
}
