#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <algorithm>
#include <cassert>

struct JunctionBox
{
    std::tuple<long long, long long, long long> location;
    int circuit_id;
};

std::vector<JunctionBox> boxes;
auto next_circuit_id = 0;

auto merge_circuits(int id1, int id2)
{
    for (auto &box : boxes)
        if (box.circuit_id == id1 || box.circuit_id == id2)
            box.circuit_id = next_circuit_id;

    next_circuit_id++;
}

auto get_distance(JunctionBox *b1, JunctionBox *b2)
{
    auto xdiff = std::get<0>(b1->location) - std::get<0>(b2->location);
    auto ydiff = std::get<1>(b1->location) - std::get<1>(b2->location);
    auto zdiff = std::get<2>(b1->location) - std::get<2>(b2->location);

    return std::sqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
}

auto do_part1(const std::string &filename, int times)
{
    std::ifstream file(filename);
    auto x = 0, y = 0, z = 0;
    auto comma = ',';

    while (file >> x >> comma >> y >> comma >> z)
        boxes.push_back({ { x, y, z }, next_circuit_id++ });

    std::vector<std::pair<std::pair<JunctionBox *, JunctionBox *>, double>> distances;

    for (auto i = 0; i < boxes.size(); i++)
        for (auto j = i + 1; j < boxes.size(); j++)
            distances.push_back({ { &boxes[i], &boxes[j]}, get_distance(&boxes[i], &boxes[j]) });

    std::sort(distances.begin(), distances.end(), [](const std::pair<std::pair<JunctionBox *, JunctionBox *>, double> &a, const std::pair<std::pair<JunctionBox *, JunctionBox *>, double> &b) -> bool {
        return a.second > b.second;
        });

    auto circuit_sizes = std::vector<int>();

    for (auto i = 0; i < times; i++)
    {
        auto closest1 = distances.back().first.first, closest2 = distances.back().first.second;
        distances.pop_back();

        if (closest1->circuit_id == closest2->circuit_id)
            continue;

        merge_circuits(closest1->circuit_id, closest2->circuit_id);
    }
        
    for (auto j = 0; j < next_circuit_id; j++)
    {
        auto count = 0;
        for (auto &box : boxes)
            if (box.circuit_id == j)
                count++;

        if (count)
            circuit_sizes.push_back(count);
    }

    std::sort(circuit_sizes.begin(), circuit_sizes.end(), std::greater<int>());

    return circuit_sizes[0] * circuit_sizes[1] * circuit_sizes[2];
}

bool all_same_circuit()
{
    for (auto box : boxes)
        if (box.circuit_id != boxes[0].circuit_id)
            return false;
    return true;
}

auto do_part2(const std::string &filename)
{
    boxes.clear();
    next_circuit_id = 0;

    std::ifstream file(filename);
    auto x = 0, y = 0, z = 0;
    auto comma = ',';

    while (file >> x >> comma >> y >> comma >> z)
        boxes.push_back({ { x, y, z }, next_circuit_id++ });

    std::vector<std::pair<std::pair<JunctionBox *, JunctionBox *>, double>> distances;

    for (auto i = 0; i < boxes.size(); i++)
        for (auto j = i + 1; j < boxes.size(); j++)
            distances.push_back({ { &boxes[i], &boxes[j]}, get_distance(&boxes[i], &boxes[j]) });

    std::sort(distances.begin(), distances.end(), [](const std::pair<std::pair<JunctionBox *, JunctionBox *>, double> &a, const std::pair<std::pair<JunctionBox *, JunctionBox *>, double> &b) -> bool {
        return a.second > b.second;
        });

    auto circuit_sizes = std::vector<int>();

    while (!distances.empty())
    {
        auto closest1 = distances.back().first.first, closest2 = distances.back().first.second;
        distances.pop_back();

        if (closest1->circuit_id == closest2->circuit_id)
            continue;

        merge_circuits(closest1->circuit_id, closest2->circuit_id);

        if (all_same_circuit())
            return std::get<0>(closest1->location) * std::get<0>(closest2->location);
    }

    return 0LL;
}

int main()
{
    auto part1 = do_part1("input.txt", 1000);
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 175500);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 6934702555);

    return 0;
}
