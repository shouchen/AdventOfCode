#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

bool process_line(std::string &data, std::vector<int> &groups)
{
    bool in_group = false;
    int count = 0;

    int group_index = 0;

    for (int i = 0; i < data.size(); i++)
    {
        if (data[i] == '#')
        {
            if (in_group)
                count++;
            else
            {
                count = 1;
                in_group = true;
            }
        }
        else
        {
            if (in_group)
            {
                if (group_index >= groups.size()) return false;

                if (count != groups[group_index++])
                    return false;
                in_group = false;
                count = 0;
            }
        }
    }

    if (count)
    {
        if (group_index >= groups.size()) return false;
        return count == groups[group_index] && group_index == groups.size() - 1;
    }

    return group_index == groups.size();
}

long long total = 0;

void test_one(std::string &data, std::vector<int> &groups)
{
    //if (data == "?###????????")
    //    cout << endl;

    auto qm = data.find('?');
    if (qm == std::string::npos)
    {
        bool pl = process_line(data, groups);
        //cout << data << " " << pl << endl;

        if (process_line(data, groups))
        {
            //cout << data << endl;
            total++;
        }
        return;
    }

    data[qm] = '.';
    test_one(data, groups);
    data[qm] = '#';
    test_one(data, groups);
    data[qm] = '?';
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string data;
    int n;
    char comma;
    
    long long part1 = 0;

    while (file >> data)
    {
        std::vector<int> groups;
        for (;;)
        {
            file >> n;
            groups.push_back(n);
            if (file.peek() == ',')
                file >> comma;
            else
                break;
        }

        total = 0;
        test_one(data, groups);
//        cout << data << " " << total << endl;
        part1 += total;
    }

    return part1;
}

// count solutions with wildcards
void recur(std::string &data, std::vector<int> &groups, int index, std::vector<int> in_groups)
{
    


    //switch (data[index])
    //{
    //case '.':
    //    in_group = false;
    //    break;
    //case '#':
    //    if (in_group)
    //        count_in_group++
    //    break;
    //case '?':
    //    break;
    //default:
    //    assert(false);
    //}

}

// ? --> "." or "#", where # 
//


std::string gdata;
std::vector<int> ggroups;

long long count_matches(int input_index, std::vector<int> &input_groups)
{
    // stop recursing
    if (input_index == gdata.length())
    {
        if (input_groups.size() == ggroups.size() &&
            input_groups.back() == ggroups.back())
        {
            return 1;
        }
        else
            return 0;
    } 

    // fork to recurse
    if (gdata[input_index] == '?')
    {
        gdata[input_index] = '#';
        auto a = count_matches(input_index, input_groups);

        gdata[input_index] = '.';
        auto b = count_matches(input_index, input_groups);

        gdata[input_index] = '?';
        return a + b;
    }

    auto prev = input_index ? gdata[input_index - 1] : '.';
    char c = gdata[input_index];

    if (c == '#')
    {
        if (prev == '.')
        {
            if (input_groups.size() + 1 > ggroups.size()) // too many groups
                return 0;
            input_groups.push_back(1);
        }
        else if (prev == '#')
        {
            if (input_groups.back() + 1 > ggroups[input_groups.size() - 1]) // too many in current group
                return 0;
            input_groups.back()++;
        }
    }
    else if (c == '.')
    {
        if (prev == '#')
        {
            if (input_groups.back() != ggroups[input_groups.size() - 1]) // group ended and isn't right count
                return 0;
        }
    }

    // recurse with rest of input
    auto retval = count_matches(input_index + 1, input_groups);

    // clean up this step so we don't copy so much onto stack
    if (c == '#')
    {
        if (prev == '.')
        {
            input_groups.pop_back();
        }
        else if (prev == '#')
        {
            input_groups.back()--;
        }
    }

    return retval;
}

using Memo = std::vector<std::vector<std::vector<long long>>>;

long long recur(int s_index, int v_index, int curr_run_len, std::string &s, std::vector<int> v, Memo &memo)
{
    if (s_index >= s.size())
        return v_index == v.size();

    // If we already have memoized result, just return that
    auto &res = memo[s_index][v_index][curr_run_len];
    if (res != -1)
        return res;

    res = 0;

    // Case 1: place a '.' (reset curr_run_len)
    if ((s[s_index] == '.' || s[s_index] == '?') && curr_run_len == 0)
        res += recur(s_index + 1, v_index, 0, s, v, memo);

    // Case 2: place a '#' (increment cur / increment v_index and reset cur)
    if ((s[s_index] == '#' || s[s_index] == '?') && v_index < v.size())
    {
        if (curr_run_len + 1 == v[v_index])
        {
            if (s_index + 1 == s.size() || s[s_index + 1] != '#')
                res += recur(s_index + 2, v_index + 1, 0, s, v, memo);
        }
        else
            res += recur(s_index + 1, v_index, curr_run_len + 1, s, v, memo);
    }

    return res;
}

auto solve(std::string s, std::vector<int> v)
{
    // memo space is small! [size of input str][# blocks + 1][max run len + 1]
    auto max_run_len = *max_element(v.begin(), v.end());
    Memo memo(
        s.size(),
        std::vector<std::vector<long long>>(
            v.size() + 1,
            std::vector<long long>(max_run_len + 1, -1)));

    return recur(0, 0, 0, s, v, memo);
}

auto do_part2(const std::string &filename, bool part2)
{
    std::ifstream file(filename);
    char comma = ',';
    int n = 0;
    auto total = 0LL;
    std::string gdata;
    std::vector<int> ggroups;

    while (file >> gdata)
    {
        auto temp = gdata;

        ggroups.clear();
        for (;;)
        {
            file >> n;
            ggroups.push_back(n);
            if (file.peek() == ',')
                file >> comma;
            else
                break;
        }

        if (part2)
        {
            gdata = temp + "?" + temp + "?" + temp + "?" + temp + "?" + temp;
            auto g = ggroups;
            for (int i = 0; i < 4; i++)
                for (auto x : g)
                    ggroups.push_back(x);
        }

        total += solve(gdata, ggroups);
    }

    return total;
}

int main()
{
    auto part1 = do_part2("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 7032);
    assert(part2 == 1493340882140);
    return 0;
}
