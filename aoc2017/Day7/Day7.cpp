#include "stdafx.h"
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

struct Node
{
    std::string name;
    int weight;
    std::vector<std::string> children;
};

std::map<std::string, Node *> nodemap;
std::set<std::string> childlist;

// returns the total weight of the node passed in plus children
int compute_child_weights(const std::string &name)
{
    Node *node = nodemap[name];

    int weight = node->weight;
    for (auto child_name : node->children)
        weight += compute_child_weights(child_name);

    return weight;
}

// Returns the name of the child node that has a different weight than its siblings,
// or "" if they are all equal. Q: What if exactly 2 and they are different? Which one
// is wrong?
std::string find_different_child_weight(const std::string &name, int &diff)
{
    Node *node = nodemap[name];
    if (node->children.size() > 2)
    {
        std::map<int, int> weight_count;
        std::vector<int> child_weights;
        for (auto iter = node->children.begin(); iter != node->children.end(); iter++)
            child_weights.push_back(compute_child_weights(*iter));

        int wrong;
        std::sort(child_weights.begin(), child_weights.end());
        auto first = child_weights[0], second = child_weights[1];
        auto next_to_last = child_weights[child_weights.size() - 2], last = child_weights[child_weights.size() - 1];

        if (first == second && second != last)
            wrong = last;
        else if (next_to_last == last && last != first)
            wrong = first;
        else
        {
            diff = 0;
            return "";
        }

        for (auto iter = node->children.begin(); iter != node->children.end(); iter++)
        {
            auto temp = compute_child_weights(*iter);
            if (temp == wrong)
            {
                if (iter == node->children.begin())
                    diff = wrong - compute_child_weights(node->children[node->children.size() - 1]);
                else
                    diff = wrong - compute_child_weights(*node->children.begin());
                return *iter;
            }
        }
    }

    diff = 0;
    return "";
}

void do_processing(const std::string &filename, std::string &part1, int &part2)
{
    nodemap.clear();
    childlist.clear();

    std::ifstream f(filename);
    std::string line;

    while (std::getline(f, line))
    {
        std::stringstream ss(line);
        char c;

        Node *node = new Node();
        ss >> node->name >> c >> node->weight >> c;
        nodemap[node->name] = node;

        std::string arrow;
        if ((ss >> arrow) && arrow == "->")
        {
            std::string name;
            while (ss >> name)
            {
                if (name[name.size() - 1] == ',')
                    name = name.substr(0, name.size() - 1); // remove comma
                node->children.push_back(name);
                childlist.insert(name);
            }
        }
    }

    // Part 1: Find node that doesn't appear on any children
    for (auto iter : nodemap)
    {
        if (childlist.find(iter.second->name) == childlist.end())
        {
            part1 = iter.second->name;
            break;
        }
    }

    // Part 2: Follow the tree from the root. At each node, if there's an imbalance, go to the 
    // one-off child. Stop when balanced. This should get to sphbbz.
    std::string curr = part1;
    for (;;)
    {
        int diff = 0;
        std::string next = find_different_child_weight(curr, diff);
        if (next == "")
        {
            int diff = 0;
            find_different_child_weight(part1, diff);

            part2 = nodemap[curr]->weight - diff;
            return;
        }

        curr = next;
    }
}

int main()
{
    std::string part1;
    int part2 = 0;

    do_processing("input-test.txt", part1, part2);
    assert(part1 == "tknk");
    assert(part2 = 60);

    do_processing("input.txt", part1, part2);
    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == "svugo");
    assert(part2 = 1152);
    return 0;
}
