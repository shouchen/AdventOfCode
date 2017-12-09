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
    std::vector<Node *> children;
};

std::map<std::string, Node *> nodemap; // this leaks allocations, of course

// returns the total weight of the node passed in plus children
int compute_child_weights(Node *node)
{
    int weight = node->weight;
    for (auto child : node->children)
        weight += compute_child_weights(child);

    return weight;
}

// Returns the child node that has a different weight than its siblings,
// or nullptr if they are all equal. Q: What if exactly 2 and they are different? Which one
// is wrong?
Node *find_different_child_weight(Node *node, int &diff)
{
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
            return nullptr;
        }

        for (auto iter = node->children.begin(); iter != node->children.end(); iter++)
        {
            if (compute_child_weights(*iter) == wrong)
            {
                diff = wrong - compute_child_weights((iter == node->children.begin())
                    ? node->children[node->children.size() - 1] : *node->children.begin());
                return *iter;
            }
        }
    }

    diff = 0;
    return nullptr;
}

void do_processing(const std::string &filename, std::string &part1, int &part2)
{
    std::set<std::string> childlist;

    nodemap.clear();

    std::ifstream f(filename);
    std::string line;

    while (std::getline(f, line))
    {
        std::stringstream ss(line);
        char c;


        std::string name;
        auto weight = 0U;
        ss >> name >> c >> weight >> c;

        // Update existing node or create a new one
        auto node_iter = nodemap.find(name);
        Node *node = nullptr;
        if (node_iter == nodemap.end())
        {
            node = new Node();
            nodemap[name] = node;
            node->name = name;
        }

        node->weight = weight;

        std::string arrow;
        if ((ss >> arrow) && arrow == "->")
        {
            std::string name;
            while (ss >> name)
            {
                // remove trailing comma
                if (name[name.size() - 1] == ',')
                    name = name.substr(0, name.size() - 1);

                // use existing child node or create a new one
                auto child_node_iter = nodemap.find(name);
                Node *child_node = nullptr;
                if (child_node_iter == nodemap.end())
                {
                    child_node = new Node();
                    child_node->name = name;
                    child_node->weight = 0; // will be changed later when this node is read in
                }
                else
                {
                    child_node = child_node_iter->second;
                }

                node->children.push_back(child_node);
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
    auto curr = nodemap[part1];
    for (;;)
    {
        int diff = 0;
        auto next = find_different_child_weight(curr, diff);
        if (!next)
        {
            int diff = 0;
            find_different_child_weight(nodemap[part1], diff);

            part2 = curr->weight - diff;
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
