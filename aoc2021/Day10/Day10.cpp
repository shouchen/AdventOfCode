#include <iostream>
#include <fstream>
#include <strstream>
#include <vector>
#include <algorithm>
#include <cassert>

auto process_line(std::string &line)
{
    std::vector<char> stack;
    auto syntax_score = 0LL, total_score = 0LL;
    
    for (auto c : line)
    {
        switch (c)
        {
        case '{':
        case '[':
        case '<':
        case '(':
            stack.push_back(c);
            continue;
        case '}':
            if (stack.back() != '{')
                syntax_score = 1197;
            break;
        case ']':
            if (stack.back() != '[')
                syntax_score = 57;
            break;
        case '>':
            if (stack.back() != '<')
                syntax_score = 25137;
            break;
        case ')':
            if (stack.back() != '(')
                syntax_score = 3;
            break;
        }

        if (syntax_score)
            break;

        stack.pop_back();
    }

    while (!stack.empty())
    {
        total_score = total_score * 5 + std::string("([{<").find(stack.back()) + 1;
        stack.pop_back();
    }

    return std::make_pair(syntax_score, total_score);
}

auto process_file(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::vector<long long> scores;
    auto part1 = 0LL;

    while (file >> line)
    {
        auto result = process_line(line);
        part1 += result.first;
        if (result.first == 0)
            scores.push_back(result.second);
    }

    std::sort(scores.begin(), scores.end());
    auto part2 = scores[scores.size() / 2];
    return std::make_pair(part1, part2);
}

int main()
{
    auto result = process_file("input.txt");

    std::cout << "Part One: " << result.first << std::endl;
    std::cout << "Part Two: " << result.second << std::endl;

    assert(result.first == 240123);
    assert(result.second == 3260812321);
    return 0;
}
