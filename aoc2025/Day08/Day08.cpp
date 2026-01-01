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
    std::tuple<int, int, int> location;
    std::vector<JunctionBox *> connections;
    int circuit_id;
};

auto boxes = std::vector<JunctionBox *>();
auto next_circuit_id = 1;
auto ignore = std::set<std::pair<JunctionBox *, JunctionBox *>>();

auto merge_circuits(int id1, int id2)
{
    for (auto &box : boxes)
        if (box->circuit_id == id1 || box->circuit_id == id2)
            box->circuit_id = next_circuit_id;

    next_circuit_id++;
}

auto connect_closest_pair()
{
    auto min_distance = std::numeric_limits<double>::max();
    std::pair<JunctionBox *, JunctionBox*> closest_pair;

    for (auto i : boxes)
    {
        for (auto j : boxes)
        {
            if (i == j)
                continue;

            if (ignore.find({ i, j }) != ignore.end())
                continue;

            if (std::find(i->connections.begin(), i->connections.end(), j) != i->connections.end())
                continue;

            auto xdiff = std::get<0>(i->location) - std::get<0>(j->location);
            auto ydiff = std::get<1>(i->location) - std::get<1>(j->location);
            auto zdiff = std::get<2>(i->location) - std::get<2>(j->location);

            auto distance = std::sqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);

            if (distance < min_distance)
            {
                min_distance = distance;
                closest_pair = { i, j };
            }
        }
    }

    if (closest_pair.first->circuit_id && closest_pair.first->circuit_id == closest_pair.second->circuit_id)
    {
        std::cout << "NOT Connecting boxes: ("
            << std::get<0>(closest_pair.first->location) << ", "
            << std::get<1>(closest_pair.first->location) << ", "
            << std::get<2>(closest_pair.first->location) << ") <--> ("
            << std::get<0>(closest_pair.second->location) << ", "
            << std::get<1>(closest_pair.second->location) << ", "
            << std::get<2>(closest_pair.second->location) << ") with distance "
            << min_distance << std::endl;

        ignore.insert({ closest_pair.first, closest_pair.second });
        ignore.insert({ closest_pair.second, closest_pair.first });
        return;
    }

    std::cout << "Connecting boxes: (" 
              << std::get<0>(closest_pair.first->location) << ", "
              << std::get<1>(closest_pair.first->location) << ", "
              << std::get<2>(closest_pair.first->location) << ") <--> ("
              << std::get<0>(closest_pair.second->location) << ", "
              << std::get<1>(closest_pair.second->location) << ", "
              << std::get<2>(closest_pair.second->location) << ") with distance " 
        << min_distance << std::endl;

    closest_pair.first->connections.push_back(closest_pair.second);
    closest_pair.second->connections.push_back(closest_pair.first);

    if (!closest_pair.first->circuit_id && closest_pair.second->circuit_id) // i no circuit, j has circuit
        closest_pair.first->circuit_id = closest_pair.second->circuit_id;
    else if (closest_pair.first->circuit_id && !closest_pair.second->circuit_id) // i has circuit, j doesn't
        closest_pair.second->circuit_id = closest_pair.first->circuit_id;
    else if (closest_pair.first->circuit_id && closest_pair.second->circuit_id) // both have different circuits
        merge_circuits(closest_pair.first->circuit_id, closest_pair.second->circuit_id);
    else // neither was in a circuit yet
        closest_pair.first->circuit_id = closest_pair.second->circuit_id = next_circuit_id++;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    auto x = 0, y = 0, z = 0;
    auto comma = ',';

    while (file >> x >> comma >> y >> comma >> z)
    {
        auto box = new JunctionBox{ std::make_tuple(x, y, z), {}, 0 };
        boxes.push_back(box);
    }

    auto circuit_sizes = std::vector<int>();

    for (auto i = 0; i < 1000; i++)
    {
        connect_closest_pair();

        for (auto j = 1; j < next_circuit_id; j++)
        {
            auto count = 0;
            for (auto &box : boxes)
                if (box->circuit_id == j)
                    count++;

            if (count)
            {
                std::cout << "Circuit " << j << ": " << count << std::endl;

                if (i == 999)
                    circuit_sizes.push_back(count);
            }
        }
    }

    std::sort(circuit_sizes.begin(), circuit_sizes.end(), std::greater<int>());

    return circuit_sizes[0] * circuit_sizes[1] * circuit_sizes[2];
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    // assert(part1 == ); // 23700 was too low :(

    //auto part2 = do_part2("input.txt");
    //std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
