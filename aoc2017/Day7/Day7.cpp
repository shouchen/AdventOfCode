#include "stdafx.h"
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <regex>

struct NodeData
{
    unsigned weight, cume_weight;
    std::vector<std::string> children;
};

std::string root;
std::map<std::string, NodeData> node_data;

unsigned set_cume_weights_recursively(const std::string &node_name)
{
    auto &node = node_data[node_name];
    node.cume_weight = node.weight;

    for (auto child : node.children)
        node.cume_weight += set_cume_weights_recursively(child);

    return node.cume_weight;
}

void read_input(const std::string &filename)
{
    node_data.clear();

    std::set<std::string> childset;
    std::ifstream f(filename);
    std::string token, name;

    while (f >> token)
    {
        if (token == "->")
        {
            auto eol = false;
            std::string child_name;

            while (!eol && f >> child_name)
            {
                if (child_name[child_name.length() - 1] == ',')
                    child_name = child_name.substr(0, child_name.length() - 1);
                else
                    eol = true;

                node_data[name].children.push_back(child_name);
                childset.insert(child_name);
            }
        }
        else
        {
            name = token;
            auto &data = node_data[name];

            auto openParenth = '(', closeParenth = ')';
            f >> openParenth >> data.weight >> closeParenth;
            data.cume_weight = 0;
        }
    }

    // Find root node (the one that doesn't appear as child of another one).
    for (auto iter : node_data)
    {
        if (childset.find(iter.first) == childset.end())
        {
            root = iter.first;
            break;
        }
    }

    set_cume_weights_recursively(root);
}

auto find_oddball_child_node(const std::string &node_name, int &diff)
{
    std::string child;
    diff = 0;

    std::map<unsigned, unsigned> weight_to_count;
    auto &node = node_data[node_name];

    for (auto &child : node.children)
    {
        auto child_node = node_data[child];
        weight_to_count[child_node.cume_weight]++;
    }

    if (weight_to_count.size() == 2)
    {
        // Find oddball weight
        auto common_weight = 0U, oddball_weight = 0U;
        for (auto i : weight_to_count)
        {
            if (i.second == 1)
                oddball_weight = i.first;
            else
                common_weight = i.first;
        }

        // Find oddball node
        for (auto iter : node.children)
        {
            if (node_data[iter].cume_weight == oddball_weight)
            {
                child = iter;
                diff = oddball_weight - common_weight;
                break;
            }
        }
    }

    return child;
}

auto compute_corrected_weight(const std::string &name)
{
    std::string prev;
    auto diff = 0, prev_diff = 0;

    for (auto curr = root; curr.length(); curr = find_oddball_child_node(curr, diff))
    {
        prev = curr;
        prev_diff = diff;
    }

    return node_data[prev].weight - prev_diff;
}

int main()
{
    read_input("input-test.txt");
    auto part1 = root;
    auto part2 = compute_corrected_weight(root);

    assert(part1 == "tknk");
    assert(part2 = 60);

    read_input("input.txt");
    part1 = root;
    part2 = compute_corrected_weight(root);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == "svugo");
    assert(part2 = 1152);
    return 0;
}
