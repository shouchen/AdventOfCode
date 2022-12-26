#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cassert>

struct Valve
{
    std::string name;
	int rate;
    bool is_open;
};

std::vector<Valve> valves;
std::map<std::string, int> name_to_valve_index;
int dists[64][64];	// min. distances between valves

auto find_or_create_valve_index(const std::string &name)
{
    size_t i;

    for (i = 0; i < valves.size(); i++)
        if (name == valves[i].name)
            return int(i);

    valves.push_back(Valve{ name, 0, false });
    return int(i);
}

void compute_dists(void)
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

int max_val(
    size_t pos1, int time1,	// actor 1 pos and time left
    size_t pos2, int time2)	// actor 2
{
    auto best = 0, rest = 0, val = 0;

    if (time1 <= 1 && time2 <= 1)
        return 0;

    for (auto i = 0; i < valves.size(); i++) {
        if (!valves[i].rate) continue;
        if (valves[i].is_open) continue;

        valves[i].is_open = true;

        if (time1 >= time2)
		{
            rest = time1 - dists[pos1][i] - 1;
            val = valves[i].rate * rest + max_val(i, rest, pos2, time2);
            best = std::max(best, val);
        }

        // extra check to prevent identical work
        if (time2 > time1 || (time1 == time2 && pos1 != pos2))
        {
            rest = time2 - dists[pos2][i] - 1;
            val = valves[i].rate * rest + max_val(pos1, time1, i, rest);
            best = std::max(best, val);
		}

        valves[i].is_open = false;
    }

    return best;
}

auto process_input(const std::string &filename, bool part2)
{
	std::ifstream file(filename);
	std::string line, valve, name, has, flow, tunnels, lead, to;
	auto c = ' ', semicolon = ';';
    auto rate = 0;
    std::map<std::string, int> name_to_valve_index;

	while (std::getline(file, line))
	{
		std::stringstream ss(line);

        ss >> valve >> name >> has >> flow >> c >> c >> c >> c >> c;
        ss >> rate >> semicolon >> tunnels >> lead >> to >> valve;

        auto index = find_or_create_valve_index(name);
        valves[index].rate = rate;

        while (ss >> name)
        {
            if (name.length() == 3) name.pop_back();
            dists[index][find_or_create_valve_index(name)] = 1;
        }
    }

    compute_dists();

    auto index_aa = find_or_create_valve_index("AA");
    return part2 ? max_val(index_aa, 26, index_aa, 26) : max_val(index_aa, 30, 0, 0);
}

int main()
{
    auto part1 = process_input("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = process_input("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 1474);
    assert(part2 == 2100);
    return 0;
}
