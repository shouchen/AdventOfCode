#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <ctype.h>
#include <algorithm>
#include <numeric>
#include <regex>
#include <cassert>

using namespace std;

struct Mapping
{
    long long dest_start, source_start, len;
};

vector<Mapping> mapping[7];

auto follow_map(long long map, long long value)
{
    for (long long i = 0; i < mapping[map].size(); i++)
        if (value>= mapping[map][i].source_start && value < mapping[map][i].source_start + mapping[map][i].len)
        {
            return value - mapping[map][i].source_start + mapping[map][i].dest_start;
        }

    return value;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, seeds_colon, dummy, word;
    long long n;
    vector<long long> seeds;

    // seeds:
    while (std::getline(file, line))
    {
        std::istringstream is(line);
        is >> seeds_colon;
        while (is >> n)
            seeds.push_back(n);

        std::getline(file, line);

        for (int i = 0; i < 7; i++)
        {
            std::getline(file, line);
            while (std::getline(file, line) && !line.empty())
            {
                std::istringstream is(line);
                Mapping m;
                is >> m.dest_start >> m.source_start >> m.len;
                mapping[i].push_back(m);
            }
        }
    }   

    long long min = 5000000000;

    for (auto i : seeds)
    {
        long long curr = i;
        for (int j = 0; j < 7; j++)
        {
            curr = follow_map(j, curr);
        }

        cout << i << " " << curr << endl;

        if (curr < min)
        {
            min = curr;
        }
    }

    return min;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, seeds_colon, dummy, word;
    long long n;
    vector<long long> seeds;

    // seeds:
    while (std::getline(file, line))
    {
        std::istringstream is(line);
        is >> seeds_colon;
        while (is >> n)
            seeds.push_back(n);

        std::getline(file, line);

        for (int i = 0; i < 7; i++)
        {
            std::getline(file, line);
            while (std::getline(file, line) && !line.empty())
            {
                std::istringstream is(line);
                Mapping m;
                is >> m.dest_start >> m.source_start >> m.len;
                mapping[i].push_back(m);
            }
        }
    }

    long long min = 5000000000;

    for (int i = 0; i < seeds.size(); i += 2)
    {
        cout << i << endl;
        for (int j = 0; j < seeds[i + 1]; j++)
        {
            long long curr = seeds[i] + j;
            for (int k = 0; k < 7; k++)
            {
                curr = follow_map(k, curr);
            }

            if (curr < min)
            {
                min = curr;
            }
        }
    }

    return min;
}

int main()
{
    //auto part1 = do_part1("input.txt");
    //std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    //assert(part1 == 484023871);
    //assert(part2 == 46294175);
    return 0;
}


//2276375722 2276375722
// 160148132 1438450556
//3424292843 3424292843
//  82110297 2085838405
//1692203766 2177445129
// 342813967  973298328
//3289792522 3289792522
// 103516087 2107244195
//2590548294 2590548294
// 590357761 2364016362
//1365412380 1532253501
//  80084180 2083812288
//3574751516 3574751516
// 584781136 1567398367
//4207087048 4207087048
//  36194356 2039922464
//1515742281 2200282983
// 174009980 1452312404
//   6434225 2010162333
// 291842774 1622623383