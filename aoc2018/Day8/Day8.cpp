#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <numeric>
#include <cassert>

struct Node
{
    std::vector<std::unique_ptr<Node>> children;
    std::vector<unsigned> metadata;
};

std::unique_ptr<Node> tree;

std::unique_ptr<Node> read_node_and_sum_metadata(std::istream &file, unsigned &total_metadata)
{
    std::unique_ptr<Node> node;
    auto num_children = 0U, num_metadata = 0U;

    if (file >> num_children >> num_metadata)
    {
        node = std::make_unique<Node>();

        while (num_children--)
        {
            auto child = read_node_and_sum_metadata(file, total_metadata);
            node->children.push_back(std::move(child));
        }

        while (num_metadata--)
        {
            auto metadata = 0U;
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
    auto total_metadata = 0U;

    tree = read_node_and_sum_metadata(file, total_metadata);
    return total_metadata;
}

unsigned do_part2(Node *node)
{
    if (!node->children.size())
        return std::accumulate(node->metadata.begin(), node->metadata.end(), 0);

    auto value = 0U;
    for (auto item : node->metadata)
        if (item > 0 && item <= node->children.size())
            value += do_part2(node->children[item - 1].get());
    return value;
}

int main()
{
    auto part1 = do_part1("input.txt");
    auto part2 = do_part2(tree.get());

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 42798);
    assert(part2 == 23798);
    return 0;
}
