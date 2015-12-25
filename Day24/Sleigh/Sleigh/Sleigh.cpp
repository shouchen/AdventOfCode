// Sleigh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

// Define this to 3 for Part 1, and 4 for Part 2
#define NUM_GROUPS 3

using namespace std;

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

vector<Package> packages;
Group groups[NUM_GROUPS + 1];

unsigned weightPerGroup;
vector<vector<Package>> solutions;
unsigned minSizeGroupOne = UINT_MAX;

unsigned GetWeight(unsigned group)
{
    unsigned weight = 0;
    for (auto p : packages)
        if (p.group == group)
            weight += p.weight;
    return weight;
}

unsigned long long GetQE(const vector<Package> &pkgs)
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

    groups[pkg.group].weight -= pkg.weight;
    groups[pkg.group].count--;
    pkg.group = group;
    groups[pkg.group].weight += pkg.weight;
    groups[pkg.group].count++;
}

void AddSolution(const vector<Package> &pkgs)
{
    unsigned size = groups[1].count;
    if (size > minSizeGroupOne) return;

    if (size < minSizeGroupOne)
    {
        minSizeGroupOne = size;
        solutions.clear();
    }

    solutions.push_back(pkgs);
}

bool CheckWayForGroupN(unsigned n, vector<Package>::iterator from, vector<Package>::iterator to, unsigned minPackages)
{
    if (NUM_GROUPS == n)
    {
        bool retval = groups[NUM_GROUPS].count >= minPackages;
        if (retval)
            AddSolution(packages);
        return retval;
    }

    if (from == to || groups[n].weight > weightPerGroup) return false;

    if (groups[n].weight == weightPerGroup)
    {
        if (groups[n].count < minPackages) return false;

        return CheckWayForGroupN(n + 1, packages.begin(), packages.end(), minPackages);
    }

    for (auto curr = from; curr != to; curr++)
    {
        if (curr->group == NUM_GROUPS)
        {
            AssignPackage(*curr, n);
            bool retval = CheckWayForGroupN(n, curr + 1, to, minPackages);
            AssignPackage(*curr, NUM_GROUPS);

            if (retval) return true;
        }
    }

    return false;
}

void PopulateGroup1Recurse(vector<Package>::iterator from, vector<Package>::iterator to)
{
    if (groups[1].weight > weightPerGroup) return;

    if (groups[1].weight == weightPerGroup)
    {
        CheckWayForGroupN(2, packages.begin(), packages.end(), groups[1].count);
        return;
    }

    if (groups[1].count > minSizeGroupOne) return;

    for (auto curr = from; curr != to; curr++)
    {
        AssignPackage(*curr, 1);
        PopulateGroup1Recurse(curr + 1, to);
        AssignPackage(*curr, NUM_GROUPS);
    }
}

unsigned long long GetLowestQe()
{
    auto minQe = ULLONG_MAX;
    for (auto &s : solutions)
    {
        unsigned long long qe = GetQE(s);
        minQe = min(minQe, qe);
    }

    return minQe;
}

void _tmain(int argc, _TCHAR *argv[])
{
    ifstream f("Input.txt");
    unsigned weight;
    while (f >> weight)
        packages.push_back(Package{ weight, NUM_GROUPS });

    // TODO: This should really be a reverse sort
    std::reverse(packages.begin(), packages.end());

    // Everything starts in last group by default
    for (auto p : packages)
        groups[NUM_GROUPS].weight += p.weight;
    weightPerGroup = groups[NUM_GROUPS].weight / NUM_GROUPS;

    PopulateGroup1Recurse(packages.begin(), packages.end());

    auto qe = GetLowestQe();
    assert((qe == 11266889531 && NUM_GROUPS == 3) || (qe == 77387711 && NUM_GROUPS == 4));
    cout << "part one/two: " << qe << endl;
}
