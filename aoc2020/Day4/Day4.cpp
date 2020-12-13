#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <regex>
#include <map>

auto validate(std::map <std::string, std::string> &passport, bool validate_values)
{
    if (passport.find("byr") == passport.end() ||
        passport.find("iyr") == passport.end() ||
        passport.find("eyr") == passport.end() ||
        passport.find("hgt") == passport.end() ||
        passport.find("hcl") == passport.end() ||
        passport.find("ecl") == passport.end() ||
        passport.find("pid") == passport.end())
        return false;

    if (!validate_values)
        return true;

    auto byr = atoi(passport["byr"].c_str()), iyr = atoi(passport["iyr"].c_str()), eyr = atoi(passport["eyr"].c_str());
    if (byr < 1920 || byr > 2002 || iyr < 2010 || iyr > 2020 || eyr < 2020 || eyr > 2030)
        return false;

    std::smatch sm;
    if (std::regex_match(passport["hgt"], sm, std::regex("^(\\d*)in$")))
    {
        auto n = atoi(sm[1].str().c_str());
        if (n < 59 || n > 76)
            return false;
    }
    else if (std::regex_match(passport["hgt"], sm, std::regex("^(\\d*)cm$")))
    {
        auto n = atoi(sm[1].str().c_str());
        if (n < 150 || n > 193)
            return false;
    }
    else
    {
        return false;
    }

    return
        std::regex_match(passport["hcl"], std::regex("^#[0-9a-f]{6}$")) &&
        std::regex_match(passport["ecl"], std::regex("^(amb|blu|brn|gry|grn|hzl|oth)$")) &&
        std::regex_match(passport["pid"], std::regex("^\\d{9}$"));
}

auto read_passport(std::ifstream &in)
{
    std::map <std::string, std::string> passport;

    for (;;)
    {
        std::string name, value;
        auto c = ' ';;

        while (in >> std::noskipws >> c)
        {
            if (c == ':' || c == '\n')
                break;
            name.push_back(c);
        }

        if (name.empty())
            break;

        while (in >> std::noskipws >> c)
        {
            if (c == ' ' || c == '\n')
                break;
            value.push_back(c);
        }

        passport[name] = value;
    }

    return passport;
}

auto process_input(const std::string &filename, bool part2)
{
    std::ifstream file(filename);
    auto count = 0;

    while (!file.eof())
    {
        auto passport = read_passport(file);

        if (validate(passport, part2))
            count++;
    }

    return count;
}

int main()
{
    auto part1 = process_input("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = process_input("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 182);
    assert(part2 == 109);
    return 0;
}
