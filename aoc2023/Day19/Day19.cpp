#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

#pragma region types
using Range = std::pair<int, int>;
using Ranges = std::vector<std::pair<int, int>>;

struct Part
{
    int x, m, a, s;
};

struct PartRange
{
    Ranges x{ {1,4000} }, m{ {1,4000} }, a{ {1,4000} }, s{ {1,4000} };

    auto get_size(Range &r)
    {
        return r.second - r.first + 1;
    }

    auto get_size(Ranges &ranges)
    {
        auto size = 0;
        for (auto &r : ranges)
            size += get_size(r);
        return size;
    }

    auto get_size()
    {
        return (long long)get_size(x) * get_size(m) * get_size(a) * get_size(s);
    }
};

struct Rule
{
    char xmas = ' ', op = ' '; // x,m,a,s or space if no op; lt, gt, or space if no op
    int operand = 0; // operand or zero if no op
    std::string action;
};

struct Workflow
{
    std::string label;
    std::vector<Rule> rules;
};
#pragma endregion

std::map<std::string, Workflow> workflows;

#pragma region Parsing
auto parse_rule(std::string line)
{
    Rule retval;

    if (line.find('<') != std::string::npos || line.find('>') != std::string::npos)
    {
        retval.xmas = line[0];
        retval.op = line[1];
        retval.operand = stoi(line.substr(2));

        auto colon = line.find(':');
        retval.action = line.substr(colon + 1);
    }
    else
    {
        retval.action = line;
    }

    return retval;
}

auto parse_rules(std::string line)
{
    std::vector<Rule> retval;

    auto curr = 0;
    for (;;)
    {
        auto comma = line.find(',', curr);
        if (comma == std::string::npos)
        {
            Rule r = parse_rule(line.substr(curr));
            retval.push_back(r);
            break;
        }
        else
        {
            Rule r = parse_rule(line.substr(curr, comma - curr));
            retval.push_back(r);
            curr = int(comma + 1);
        }
    }

    return retval;
}

auto parse_workflow(std::string line)
{
    auto brace1 = line.find('{'), brace2 = line.find('}');

    return Workflow{
        line.substr(0, brace1),
        parse_rules(line.substr(brace1 + 1, brace2 - brace1 - 1))
    };
}

auto parse_part(std::string line)
{
    Part part;
    part.x = stoi(line.substr(line.find("x=") + 2));
    part.m = stoi(line.substr(line.find("m=") + 2));
    part.a = stoi(line.substr(line.find("a=") + 2));
    part.s = stoi(line.substr(line.find("s=") + 2));
    return part;
}
#pragma endregion

bool is_accepted(Part &part)
{
    auto curr = workflows["in"];

    for (;;)
    {
        for (auto &r : curr.rules)
        {
            if (r.op == '<')
            {
                if (r.xmas == 'x' && part.x < r.operand ||
                    r.xmas == 'm' && part.m < r.operand ||
                    r.xmas == 'a' && part.a < r.operand ||
                    r.xmas == 's' && part.s < r.operand)
                {
                    if (r.action == "A")
                        return true;
                    else if (r.action == "R")
                        return false;

                    curr = workflows[r.action];
                    break;
                }
            }
            else if (r.op == '>')
            {
                if (r.xmas == 'x' && part.x > r.operand ||
                    r.xmas == 'm' && part.m > r.operand ||
                    r.xmas == 'a' && part.a > r.operand ||
                    r.xmas == 's' && part.s > r.operand)
                {
                    if (r.action == "A")
                        return true;
                    else if (r.action == "R")
                        return false;

                    curr = workflows[r.action];
                    break;
                }
            }
            else if (r.action == "A")
            {
                return true;
            }
            else if (r.action == "R")
            {
                return false;
            }
            else
            {
                curr = workflows[r.action];
                break;
            }
        }
    }
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line) && !line.empty())
    {
        auto w = parse_workflow(line);
        workflows[w.label] = w;
    }

    auto retval = 0LL;

    while (std::getline(file, line) && !line.empty())
    {
        Part p = parse_part(line);
        if (is_accepted(p))
            retval += p.x + p.m + p.a + p.s;
    }

    return retval;
}

auto intersect(Range &r1, Range &r2) // returns -1,-1 if no intersection
{
    if (r1.first > r2.second || r1.second < r2.first) // no intersection
        return Range{ -1,-1 };
    if (r1.first >= r2.first && r1.second <= r2.second) // return full r1
        return r1;
    if (r1.first >= r2.first && r1.first < r2.second)  // return left part of r1
        return Range{ r1.first, r2.second };
    if (r2.first > r1.first && r2.first <= r1.second && r2.second >= r1.second)  // return right part of r1
        return Range{ r2.first, r1.second };
    return r2;
}

Ranges intersect(Ranges ranges, Range r)
{
    Ranges retval;
    for (auto &i : ranges)
    {
        auto inter = intersect(i, r);
        if (inter.first != -1)
            retval.push_back(inter);
    }
    return retval;
}

Ranges sub_intersects(Ranges ranges, Range r)
{
    Ranges retval;
    for (auto &i : ranges)
    {
        auto inter = intersect(i, r);

        if (inter.first == -1)
            retval.push_back(inter);
        else
        {
            bool has_left_non_intersect = i.first < inter.first;
            bool has_right_non_intersect = i.second > inter.second;

            if (has_left_non_intersect)
                retval.push_back(Range{ i.first, inter.first - 1 });

            if (has_right_non_intersect)
                retval.push_back(Range{ inter.second + 1, i.second });
        }
    }
    return retval;
}

long long recur(const std::string &label, PartRange pr)
{
    if (label == "A")
        return pr.get_size();

    if (label == "R")
        return 0;

    auto retval = 0LL;
    auto &wf = workflows[label];

    for (auto &r : wf.rules)
    {
        PartRange new_pr = pr;

        // whittle down range if op
        if (r.op != ' ')
        {
            assert(r.op == '<' || r.op == '>');
            auto temp = (r.op == '<') ? Range{ 1, r.operand - 1 } : Range{ r.operand + 1, 4000 };

            switch (r.xmas)
            {
            case 'x':
                new_pr.x = intersect(pr.x, temp);
                pr.x = sub_intersects(pr.x, temp);
                break;
            case 'm':
                new_pr.m = intersect(pr.m, temp);
                pr.m = sub_intersects(pr.m, temp);
                break;
            case 'a':
                new_pr.a = intersect(pr.a, temp);
                pr.a = sub_intersects(pr.a, temp);
                break;
            case 's':
                new_pr.s = intersect(pr.s, temp);
                pr.s = sub_intersects(pr.s, temp);
                break;
            default:
                assert(false);
            }
        }

        retval += recur(r.action, new_pr);
    }
    return retval;
}

auto do_part2()
{
    return recur("in", PartRange());
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 362930);
    assert(part2 == 116365820987729);
    return 0;
}
