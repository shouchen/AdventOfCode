// Sleigh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

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
Group groups[4];

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

unsigned GetCount(unsigned group, const vector<Package> pkgs)
{
    unsigned count = 0;
    for (auto p : pkgs)
        if (p.group == group)
            count++;
    return count;
}

unsigned long long GetQE(const vector<Package> &pkgs)
{
    unsigned long long qe = 1;
    for (auto p : pkgs)
        if (p.group == 1)
            qe *= p.weight;
    return qe;
}

void AssignPackage(Package &pkg, unsigned group)
{
    groups[pkg.group].weight -= pkg.weight;
    pkg.group = group;
    groups[pkg.group].weight += pkg.weight;
}

void AddSolution(const vector<Package> &pkgs)
{
    unsigned size = GetCount(1, pkgs);
    if (size > minSizeGroupOne) return;

    if (size < minSizeGroupOne)
    {
        // I think this should only ever happen once if reverse sorted
        cout << "NEW SOLUTION OF SIZE " << size << endl;
        minSizeGroupOne = size;
        solutions.clear();
    }

    solutions.push_back(pkgs);
}

void Dump(const vector<Package> &pkgs)
{
    cout << GetQE(pkgs) << " ( ";
    for (auto p : pkgs)
        if (p.group == 1) cout << p.weight << " ";
        cout << ") ( ";
        for (auto p : pkgs)
        if (p.group == 2) cout << p.weight << " ";
    cout << ") ( ";
    for (auto p : pkgs)
        if (p.group == 3) cout << p.weight << " ";
    cout << ")" << endl;
}

// Arrange all packages into three equal groups
// Group 1 needs minimal # packages
// For tiebreaker, minimize product in group 1. Only apply this if 

bool CheckWayForGroupsTwoAndThreeRecurse(vector<Package>::iterator from, vector<Package>::iterator to, unsigned minPackages)
{
    if (from == to || groups[2].weight > weightPerGroup) return false;

    if (groups[2].weight == weightPerGroup)
    {
        bool retval = GetCount(2, packages) >= minPackages && GetCount(3, packages) >= minPackages;
        if (retval)
            AddSolution(packages);
        return retval;
    }

    for (auto curr = from; curr != to; curr++)
    {
        if (curr->group == 3)
        {
            AssignPackage(*curr, 2);
            bool retval = CheckWayForGroupsTwoAndThreeRecurse(curr + 1, to, minPackages);
            AssignPackage(*curr, 3);

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
        Dump(packages);

        CheckWayForGroupsTwoAndThreeRecurse(packages.begin(), packages.end(), GetCount(1, packages));
        return;
    }

    if (GetCount(1, packages) > minSizeGroupOne) return;

    for (auto curr = from; curr != to; curr++)
    {
        AssignPackage(*curr, 1);
        PopulateGroup1Recurse(curr + 1, to);
        AssignPackage(*curr, 3);
    }
}

unsigned long long GetLowestQe()
{
    unsigned long long minQe = ULLONG_MAX;
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
        packages.push_back(Package{ weight, 3 });

    // This should really be a reverse sort
    std::reverse(packages.begin(), packages.end());

    // Everything starts in group 3 by default
    for (auto p : packages)
        groups[3].weight += p.weight;
    weightPerGroup = groups[3].weight / 3;

    PopulateGroup1Recurse(packages.begin(), packages.end());

    cout << "SOLUTIONS WITH LOWEST GROUP ONE COUNT: " << endl;
    for (auto &s : solutions) Dump(s);

    auto qe = GetLowestQe();
    assert(qe == 11266889531);
    cout << "part one: " << qe << endl;
}
