#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <cassert>

struct Node
{
    std::vector<Node *> children;
    std::vector<unsigned> metadata;
} *tree;

unsigned total_metadata = 0;

Node *read_node(std::ifstream &file)
{
    auto num_children = 0U, num_metadata = 0U;

    Node *node = NULL;

    if (file >> num_children >> num_metadata)
    {
        node = new Node();

        for (auto i = 0U; i < num_children; i++)
            node->children.push_back(read_node(file));

        auto metadata = 0U;
        for (auto i = 0U; i < num_metadata; i++)
        {
            file >> metadata;

            node->metadata.push_back(metadata);
            total_metadata += metadata;
        }
    }

    return node;
}

unsigned do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    tree = read_node(file);
    return total_metadata;
}

unsigned do_part2(Node *node)
{
    if (!node->children.size())
        return std::accumulate(node->metadata.begin(), node->metadata.end(), 0);

    auto total = 0U;
    for (auto item : node->metadata)
        if (item > 0 && item <= node->children.size())
            total += do_part2(node->children[item - 1]);
    return total;
}

int main()
{
    auto part1 = do_part1("input.txt");
    auto part2 = do_part2(tree);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 42798);
    assert(part2 == 23798);
    return 0;
}
