// Sleigh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

#define NUM_GROUPS 4

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

bool CheckWayForGroupFour(vector<Package>::iterator from, vector<Package>::iterator to, unsigned minPackages)
{
    bool retval = groups[NUM_GROUPS].count >= minPackages;
    if (retval)
        AddSolution(packages);
    return retval;
}

bool CheckWayForGroupThree(vector<Package>::iterator from, vector<Package>::iterator to, unsigned minPackages)
{
    if (NUM_GROUPS == 3)
    {
        bool retval = groups[NUM_GROUPS].count >= minPackages;
        if (retval)
            AddSolution(packages);
        return retval;
    }

    if (from == to || groups[3].weight > weightPerGroup) return false;

    if (groups[3].weight == weightPerGroup)
    {
        if (groups[3].count < minPackages) return false;

        return CheckWayForGroupFour(packages.begin(), packages.end(), minPackages);
    }

    for (auto curr = from; curr != to; curr++)
    {
        if (curr->group == NUM_GROUPS)
        {
            AssignPackage(*curr, 3);
            bool retval = CheckWayForGroupThree(curr + 1, to, minPackages);
            AssignPackage(*curr, NUM_GROUPS);

            if (retval) return true;
        }
    }

    return false;
}

bool CheckWayForGroupTwo(vector<Package>::iterator from, vector<Package>::iterator to, unsigned minPackages)
{
    if (from == to || groups[2].weight > weightPerGroup) return false;

    if (groups[2].weight == weightPerGroup)
    {
        if (groups[2].count < minPackages) return false;

        return CheckWayForGroupThree(packages.begin(), packages.end(), minPackages);
    }

    for (auto curr = from; curr != to; curr++)
    {
        if (curr->group == NUM_GROUPS)
        {
            AssignPackage(*curr, 2);
            bool retval = CheckWayForGroupTwo(curr + 1, to, minPackages);
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
        Dump(packages);

        CheckWayForGroupTwo(packages.begin(), packages.end(), groups[1].count);
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

    // This should really be a reverse sort
    std::reverse(packages.begin(), packages.end());

    // Everything starts in last group by default
    for (auto p : packages)
        groups[NUM_GROUPS].weight += p.weight;
    weightPerGroup = groups[NUM_GROUPS].weight / NUM_GROUPS;

    PopulateGroup1Recurse(packages.begin(), packages.end());

    cout << "SOLUTIONS WITH LOWEST GROUP ONE COUNT: " << endl;
    for (auto &s : solutions) Dump(s);

    auto qe = GetLowestQe();
    assert((qe == 11266889531 && NUM_GROUPS == 3) || (qe = 77387711 && NUM_GROUPS == 4));
    cout << "part one/two: " << qe << endl;
}
