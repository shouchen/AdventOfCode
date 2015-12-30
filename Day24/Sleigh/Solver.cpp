// Sleigh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cassert>
#include "Solver.h"

unsigned Solver::s_numGroups;
unsigned Solver::s_lowestSizedGroupOne;
unsigned Solver::s_weightPerGroup;

std::vector<Solver::Package> Solver::s_packages;
Solver::Group *Solver::s_groups;
std::vector<std::vector<Solver::Package>> Solver::s_solutions;

unsigned long long Solver::Solve(std::vector<unsigned> weights, unsigned numGroups)
{
    // By sorting to heaviest first, a balanced group one will have fewer packages.
    std::sort(weights.begin(), weights.end(), std::greater<int>());
    auto totalWeight = std::accumulate(weights.begin(), weights.end(), 0U);

    // Initialize statics (resetting any state).
    s_numGroups = numGroups;
    s_lowestSizedGroupOne = UINT_MAX;
    s_weightPerGroup = totalWeight / s_numGroups;

    s_packages.clear();
    s_groups = new Group[numGroups + 1];
    s_solutions.clear();

    for (auto i = 1U; i <= numGroups; i++)
        s_groups[i].count = s_groups[i].weight = 0;

    // Default everything initially to the last group, so we can place the others.
    for (auto weight : weights)
        s_packages.push_back(Solver::Package{ weight, numGroups });

    s_groups[s_numGroups].weight = totalWeight;

    // Populate the first group recursively, and the others after it.
    PopulateGroups(s_packages.begin(), s_packages.end());

    delete[] s_groups;
    return GetLowestGroupOneQeOfSolutions();
}

void Solver::PopulateGroups(std::vector<Package>::iterator from, std::vector<Package>::iterator to)
{
    // If we already have a solution with a smaller group one, stop looking at this tree.
    if (s_groups[1].count > s_lowestSizedGroupOne) return;

    if (s_groups[1].weight > s_weightPerGroup)
    {
        // Not a solution if first group is already too heavy.
        return;
    }
    else if (s_groups[1].weight < s_weightPerGroup)
    {
        // Recursively generate all the possible group one combinations.
        for (auto curr = from; curr != to; curr++)
        {
            AssignPackage(*curr, 1);
            PopulateGroups(curr + 1, to);
            AssignPackage(*curr, s_numGroups);
        }
    }
    else
    {
        // Group one exactly hit the target weight; see if there's a way for the other groups to work.
        CheckWayForGroupNAndBeyond(2, s_packages.begin(), s_packages.end());
    }
}

bool Solver::CheckWayForGroupNAndBeyond(unsigned n, std::vector<Package>::iterator from, std::vector<Package>::iterator to)
{
    if (n == s_numGroups)
    {
        // This is the last group; make sure it has at least as many packages as group one.
        if (s_groups[n].count < s_groups[1].count) return false;

        // We found a solution. If this is a new smallest group one, throw away any previous solutions.
        if (s_groups[1].count < s_lowestSizedGroupOne)
        {
            s_lowestSizedGroupOne = s_groups[1].count;
            s_solutions.clear();
        }

        // Save this solution in a vector in case we need to break ties.
        s_solutions.push_back(s_packages);
        return true;
    }

    // If all packages distributed before we get to last group, or if this group is too heavy, stop tree.
    if (from == to || s_groups[n].weight > s_weightPerGroup) return false;

    // Check if group n is exactly at its target weight.
    if (s_groups[n].weight == s_weightPerGroup)
    {
        // If this group has enough packages (group one has to be the minimum), move to next group.
        return
            (s_groups[n].count >= s_groups[1].count) &&
            CheckWayForGroupNAndBeyond(n + 1, s_packages.begin(), s_packages.end());
    }

    // Iterate across the remaining packages. Move combinations from last group (unassigned) to this group.
    for (auto curr = from; curr != to; curr++)
    {
        if (curr->group == s_numGroups)
        {
            AssignPackage(*curr, n);
            auto retval = CheckWayForGroupNAndBeyond(n, curr + 1, to);
            AssignPackage(*curr, s_numGroups);

            // Just need one solution for Groups 2..max, so exit out now.
            if (retval) return true;
        }
    }

    return false;
}

void Solver::AssignPackage(Package &pkg, unsigned group)
{
    assert(pkg.group != group);

    // Move pkg from current group to the specified new group.
    s_groups[pkg.group].weight -= pkg.weight;
    s_groups[pkg.group].count--;
    pkg.group = group;
    s_groups[pkg.group].weight += pkg.weight;
    s_groups[pkg.group].count++;
}

unsigned long long Solver::GetGroupOneQe(const std::vector<Package> &pkgs)
{
    auto qe = 1ULL;
    for (auto p : pkgs)
    if (p.group == 1)
        qe *= p.weight;
    return qe;
}

unsigned long long Solver::GetLowestGroupOneQeOfSolutions()
{
    auto minQe = ULLONG_MAX;
    for (auto &s : s_solutions)
    {
        unsigned long long qe = GetGroupOneQe(s);
        minQe = std::min(minQe, qe);
    }

    return minQe;
}
