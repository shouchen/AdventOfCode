#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

auto count_wins(long long time, long long dist)
{
    auto wins = 0;

    for (auto charge = 0LL; charge <= time; charge++)
        if (charge * (time - charge) > dist)
            wins++;

    return wins;
}

auto concat_numbers(std::ifstream &file)
{
    std::string line;
    std::getline(file, line);

    auto numbers = line.substr(line.find(':') + 1);
    numbers.erase(remove_if(numbers.begin(), numbers.end(), isspace), numbers.end());
    return stoll(numbers);
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    std::getline(file, line);
    auto is_times = std::istringstream(line.substr(line.find(':') + 1));

    std::getline(file, line);
    auto is_dists = std::istringstream(line.substr(line.find(':') + 1));

    auto time = 0LL, dist = 0LL, product = 1LL;;

    while (is_times >> time && is_dists >> dist)
        product *= count_wins(time, dist);

    return product;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    auto time = concat_numbers(file);
    auto dist = concat_numbers(file);

    return count_wins(time, dist);
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 3317888);
    assert(part2 == 24655068);
    return 0;
}