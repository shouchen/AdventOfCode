#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

struct Valve
{
    std::string name;
	int rate;
    bool is_open;
};

std::vector<Valve> valves;
std::vector<std::vector<int>> dists;

auto find_valve_index(const std::string &name)
{
    for (auto i = 0; ; i++)
        if (name == valves[i].name)
            return i;
}

void compute_dists()
{
    for (;;)
    {
        auto something_changed = false;

        for (auto i = 0; i < valves.size(); i++)
            for (auto step1 = 0; step1 < valves.size(); step1++)
            {
                if (!dists[i][step1]) continue;

                for (auto step2 = 0; step2 < valves.size(); step2++)
                {
                    if (i == step2 || !dists[step1][step2]) continue;

                    auto d = dists[i][step1] + dists[step1][step2];

                    if (!dists[i][step2] || d < dists[i][step2])
                    {
                        dists[i][step2] = d;
                        something_changed = true;
                    }
                }
            }

        if (!something_changed)
            break;
    }
}

auto process_input_pass1(const std::string &filename)
{
	std::ifstream file(filename);
	std::string line, valve, name, has, flow;
	auto c = ' ';
    auto rate = 0;
        
    valves.push_back({ "AA", 0, false });

    while (std::getline(file, line))
	{
		std::stringstream ss(line);
        ss >> valve >> name >> has >> flow >> c >> c >> c >> c >> c >> rate;

        if (name == "AA")
            valves[0].rate = rate;
        else
            valves.push_back({ name, rate, false });
    }

    for (auto i = 0; i < valves.size(); i++)
        dists.push_back(std::vector<int>(valves.size(), 0));
}

auto process_input_pass2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, valve, name;

    while (std::getline(file, line))
    {
        std::stringstream ss1(line.substr(6));
        ss1 >> name;
        auto index = find_valve_index(name);

        std::stringstream ss(line.substr(line.find("to ") + 3));
        ss >> valve;

        while (ss >> name)
        {
            if (name.back() == ',') name.pop_back();
            auto other_index = find_valve_index(name);
            dists[index][other_index] = 1;
        }
    }
}

void process_input(const std::string &filename)
{
    process_input_pass1(filename);
    process_input_pass2(filename);
    compute_dists();
}

int max_val(size_t pos1, int time1, size_t pos2, int time2)
{
    if (time1 <= 1 && time2 <= 1)
        return 0;

    auto best = 0;

    for (auto i = 0; i < valves.size(); i++)
    {
        if (!valves[i].rate || valves[i].is_open)
            continue;

        valves[i].is_open = true;

        if (time1 >= time2)
        {
            auto rest = time1 - dists[pos1][i] - 1;
            auto val = valves[i].rate * rest + max_val(i, rest, pos2, time2);
            best = std::max(best, val);
        }

        if (time2 > time1 || (time1 == time2 && pos1 != pos2))
        {
            auto rest = time2 - dists[pos2][i] - 1;
            auto val = valves[i].rate * rest + max_val(pos1, time1, i, rest);
            best = std::max(best, val);
        }

        valves[i].is_open = false;
    }

    return best;
}

auto do_part(int minutes, bool two_elephants)
{
    return max_val(0, minutes, 0, two_elephants ? minutes : 0 );
}

int main()
{
    process_input("input.txt");

    auto part1 = do_part(30, false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part(26, true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 1474);
    assert(part2 == 2100);
    return 0;
}
