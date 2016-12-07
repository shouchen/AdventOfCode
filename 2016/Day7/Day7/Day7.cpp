#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

void parse_address(const std::string &address, std::vector<std::string> &outside_brackets, std::vector<std::string> &inside_brackets)
{
    outside_brackets.empty();
    inside_brackets.empty();

    for (auto curr = 0U; address[curr];)
    {
        auto open_brace = address.find('[', curr);

        if (open_brace == std::string::npos)
        {
            outside_brackets.push_back(address.substr(curr));
            curr = address.length();
        }
        else
        {
            outside_brackets.push_back(address.substr(curr, open_brace - curr));
            curr = address.find(']', open_brace + 1) + 1;
            inside_brackets.push_back(address.substr(open_brace + 1, curr - open_brace));
        }
    }
}

bool has_abba_pattern(const std::vector<std::string> &strings)
{
    for (auto s : strings)
        for (auto i = 0U; i < s.length() - 3; i++)
            if (s[i] == s[i + 3] && s[i + 1] == s[i + 2] && s[i] != s[i + 1])
                return true;

    return false;
}

bool supports_tls(std::vector<std::string> &outside_brackets, std::vector<std::string> &inside_brackets)
{
    return has_abba_pattern(outside_brackets) && !has_abba_pattern(inside_brackets);
}

bool supports_ssl(std::vector<std::string> &outside_brackets, std::vector<std::string> &inside_brackets)
{
    // find each 'aba' in the outside
    for (auto s : outside_brackets)
        for (auto i = 0U; i < s.length() - 2; i++)
            if (s[i] == s[i + 2] && s[i] != s[i + 1])
            {
                // find a corresponding 'bab' in the inside
                char target[] = { s[i + 1], s[i], s[i + 1], '\0' };
                for (auto t : inside_brackets)
                    if (t.find(target) != std::string::npos)
                        return true;
            }

    return false;
}

int main()
{
    std::ifstream f("input.txt");
    std::string address;
    auto tls_count = 0U, ssl_count = 0U;

    while (f >> address)
    {
        std::vector<std::string> outside, inside;
        parse_address(address, outside, inside);

        if (supports_tls(outside, inside)) tls_count++;
        if (supports_ssl(outside, inside)) ssl_count++;
    }

    std::cout << "Part One: " << tls_count << std::endl;
    std::cout << "Part Two: " << ssl_count << std::endl;

    assert(tls_count == 118);
    assert(ssl_count == 260);
    return 0;
}
