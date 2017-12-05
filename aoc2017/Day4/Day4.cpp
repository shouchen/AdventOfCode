#include "stdafx.h"
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>

bool validate_password(const std::string password, bool check_anagrams)
{
    std::stringstream ss(password);
    std::set<std::string> words;
    std::string word;

    while (ss >> word)
    {
        if (check_anagrams)
            std::sort(word.begin(), word.end());

        if (words.find(word) != words.end())
            return false;

        words.insert(word);
    }

    return true;
}

unsigned count_valid_passwords(const std::string &filename, bool check_anagrams)
{
    auto valid = 0U;

    std::ifstream f(filename);
    std::string password;

    while (std::getline(f, password))
    {
        if (validate_password(password, check_anagrams))
            valid++;
    }

    return valid;
}

void check_test_cases()
{
    assert(validate_password("aa bb cc dd ee", false) == true);
    assert(validate_password("aa bb cc dd aa", false) == false);
    assert(validate_password("aa bb cc dd aaa", false) == true);
    assert(count_valid_passwords("input-test.txt", false) == 2);

    assert(validate_password("abcde fghij", true) == true);
    assert(validate_password("abcde xyz ecdab", true) == false);
    assert(validate_password("a ab abc abd abf abj", true) == true);
    assert(validate_password("iiii oiii ooii oooi oooo", true) == true);
    assert(validate_password("oiii ioii iioi iiio", true) == false);
    assert(count_valid_passwords("input-test2.txt", true) == 3);
}

int main()
{
    check_test_cases();

    auto part1 = count_valid_passwords("input.txt", false);
    auto part2 = count_valid_passwords("input.txt", true);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 337);
    assert(part2 == 231);
    return 0;
}
