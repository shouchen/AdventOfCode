#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

auto start_timestamp = 0U;
std::vector<unsigned> buses;

void read_data(const std::string &filename)
{
    std::ifstream file("input.txt");
    file >> start_timestamp;

    while (!file.eof())
    {
        std::string s;
        auto c = ' ';

        while (file >> c && c != ',')
            s.push_back(c);

        buses.push_back(atoi(s.c_str()));
    }
}

auto do_part1()
{
    auto first_arrival = ULLONG_MAX;
    auto first_bus_id = 0U;

    for (auto b : buses)
    {
        if (b == 0)
            continue;

        auto arrival = (start_timestamp + b - 1) / b * b;

        if (arrival < first_arrival)
        {
            first_arrival = arrival;
            first_bus_id = b;
        }
    }

    return (first_arrival - start_timestamp) * first_bus_id;
}

auto do_part2()
{
    auto timestamp = 1ULL, increment = 1ULL;

    for (auto index = 0; index < buses.size(); index++)
    {
        if (buses[index] == 0)
            continue;

        while ((timestamp + index) % buses[index] != 0)
            timestamp += increment;

        increment *= buses[index];
    }

    return timestamp;
}

int main()
{
    read_data("input.txt");

    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 6568);
    assert(part2 == 554865447501099);
    return 0;
}
