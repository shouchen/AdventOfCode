#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cassert>

struct JunctionBox
{
    long long x, y, z;
    int circuit_id;
};

std::vector<JunctionBox> boxes;

auto get_distance(JunctionBox *b1, JunctionBox *b2)
{
    auto xdiff = b1->x - b2->x, ydiff = b1->y - b2->y, zdiff = b1->z - b2->z;
    return std::sqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
}

auto merge_circuits(int id1, int id2)
{
    for (auto &box : boxes)
        if (box.circuit_id == id2)
            box.circuit_id = id1;
}

auto all_same_circuit()
{
    for (auto &box : boxes)
        if (box.circuit_id != boxes[0].circuit_id)
            return false;

    return true;
}

auto get_product_of_top_three_circuit_sizes()
{
    auto circuit_sizes = std::vector<long long>();

    for (auto j = 0; j < boxes.size(); j++)
    {
        auto count = 0;
        for (auto &box : boxes)
            if (box.circuit_id == j)
                count++;

        if (count)
            circuit_sizes.push_back(count);
    }

    std::partial_sort(circuit_sizes.begin(), circuit_sizes.begin() + 3, circuit_sizes.end(), std::greater<long long>());
    return circuit_sizes[0] * circuit_sizes[1] * circuit_sizes[2];
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    auto x = 0, y = 0, z = 0;
    auto comma = ',';
    //auto retval = std::make_pair<long long, long long>(0, 0);
    std::pair<long long, long long> retval;

    while (file >> x >> comma >> y >> comma >> z)
        boxes.push_back({ x, y, z, int(boxes.size()) });

    using PairDistance = std::pair<std::pair<JunctionBox *, JunctionBox *>, double>;
    auto comp = [](const PairDistance &a, const PairDistance &b) -> bool {
        return a.second > b.second;
    };
    std::priority_queue<PairDistance, std::vector<PairDistance>, decltype(comp)> distances(comp);

    for (auto i = 0; i < boxes.size(); i++)
        for (auto j = i + 1; j < boxes.size(); j++)
            distances.push({ { &boxes[i], &boxes[j]}, get_distance(&boxes[i], &boxes[j]) });

    for (auto times = 1; !retval.first || !retval.second; times++)
    {
        auto jb1 = distances.top().first.first, jb2 = distances.top().first.second;
        distances.pop();

        if (jb1->circuit_id != jb2->circuit_id)
            merge_circuits(jb1->circuit_id, jb2->circuit_id);

        if (times == 1000)
            retval.first = get_product_of_top_three_circuit_sizes();

        if (all_same_circuit())
            retval.second = jb1->x * jb2->x;
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 175500);
    assert(answer.second == 6934702555);
    return 0;
}
