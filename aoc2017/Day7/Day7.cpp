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
    unsigned weight, cume_weight;
    std::vector<Node *> children;

    Node(const std::string &name, int weight) : name(name), weight(weight), cume_weight(0) {}
};

std::map<std::string, Node *> nodemap; // this leaks allocations, of course
Node *root;

// Returns nullptr if no children, or if all are equal. Also gets the difference if any.
// Q: What if exactly 2 and they are different? Which one is wrong?
Node *find_child_node_with_different_weight(Node *node, int &diff)
{
    std::map<unsigned, unsigned> weight_to_count;
    for (auto child : node->children)
        weight_to_count[child->cume_weight]++;

    // All were the same weight
    if (weight_to_count.size() == 1)
    {
        diff = 0;
        return nullptr;
    }

    // Check if there were more than two different weights
    assert(weight_to_count.size() == 2);

    unsigned common_weight, oddball_weight;
    for (auto i : weight_to_count)
    {
        if (i.second == 1)
            oddball_weight = i.first;
        else
            common_weight = i.first;
    }

    // Find oddball node
    for (auto child : node->children)
        if (child->cume_weight == oddball_weight)
        {
            diff = oddball_weight - common_weight;
            return child;
        }

    // Should never get here
    assert(false);
    diff = 0;
    return nullptr;
}

unsigned set_cume_weight(Node *node)
{
    node->cume_weight = node->weight;
    for (auto child : node->children)
        node->cume_weight += set_cume_weight(child);

    return node->cume_weight;
}

void process_input(const std::string &filename)
{
    std::set<std::string> childset;

    nodemap.clear();

    std::ifstream f(filename);
    std::string line;

    while (std::getline(f, line))
    {
        std::stringstream ss(line);
        std::string name;
        char openParenth, closeParenth;
        auto weight = 0U;
        ss >> name >> openParenth >> weight >> closeParenth;

        Node *node = nullptr;
        if (nodemap.find(name) == nodemap.end())
        {
            node = new Node(name, weight);
            nodemap[name] = node;
        }
        else
            node = nodemap[name];
        node->weight = weight;

        std::string arrow;
        if ((ss >> arrow) && arrow == "->")
        {
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
                    child_node = new Node(name, 0);
                    nodemap[name] = child_node;
                }
                else
                {
                    child_node = child_node_iter->second;
                }

                node->children.push_back(child_node);
                childset.insert(name);
            }
        }
    }

    // Find root node (the one that doesn't appear as child of another one)
    for (auto iter : nodemap)
    {
        if (childset.find(iter.second->name) == childset.end())
        {
            root = iter.second;
            break;
        }
    }

    // Set cume_weights recursively
    set_cume_weight(root);
}

std::string do_part1()
{
    return root->name;
}

// Follow the tree from the root. At each node, if there's an imbalance, go to the 
// one-off child. Stop when balanced. This should get to sphbbz.
unsigned do_part2()
{
    auto curr = root;
    int diff = 0;

    for (;;)
    {
        auto next = find_child_node_with_different_weight(curr, diff);
        if (!next)
            break;

        curr = next;
    }

    find_child_node_with_different_weight(root, diff);
    return curr->weight - diff;
}

int main()
{
    process_input("input-test.txt");
    auto part1 = do_part1();
    auto part2 = do_part2();

    assert(part1 == "tknk");
    assert(part2 = 60);

    process_input("input.txt");
    part1 = do_part1();
    part2 = do_part2();

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == "svugo");
    assert(part2 = 1152);
    return 0;
}
