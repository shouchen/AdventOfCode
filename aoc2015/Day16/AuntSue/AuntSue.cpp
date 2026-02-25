// AuntSue.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

struct Data
{
    int children, cats, samoyeds, pomeranians, akitas;
    int vizslas, goldfish, trees, cars, perfumes;
};

bool FieldsMatch(int first, int second)
{
    return first == -1 || second == -1 || first == second;
}

bool SecondFieldIsOvercounted(int first, int second)
{
    return first == -1 || second == -1 || first < second;
}

bool SecondFieldIsUndercounted(int first, int second)
{
    return first == -1 || second == -1 || first > second;
}

bool MatchesPart1(const Data &first, const Data &second)
{
    return
        FieldsMatch(first.children, second.children) &&
        FieldsMatch(first.cats, second.cats) &&
        FieldsMatch(first.samoyeds, second.samoyeds) &&
        FieldsMatch(first.pomeranians, second.pomeranians) &&
        FieldsMatch(first.akitas, second.akitas) &&
        FieldsMatch(first.vizslas, second.vizslas) &&
        FieldsMatch(first.goldfish, second.goldfish) &&
        FieldsMatch(first.trees, second.trees) &&
        FieldsMatch(first.cars, second.cars) &&
        FieldsMatch(first.perfumes, second.perfumes);
}

bool MatchesPart2(const Data &first, const Data &second)
{
    return
        FieldsMatch(first.children, second.children) &&
        SecondFieldIsUndercounted(first.cats, second.cats) &&
        FieldsMatch(first.samoyeds, second.samoyeds) &&
        SecondFieldIsOvercounted(first.pomeranians, second.trees) &&
        FieldsMatch(first.akitas, second.akitas) &&
        FieldsMatch(first.vizslas, second.vizslas) &&
        SecondFieldIsOvercounted(first.goldfish, second.goldfish) &&
        SecondFieldIsUndercounted(first.trees, second.trees) &&
        FieldsMatch(first.cars, second.cars) &&
        FieldsMatch(first.perfumes, second.perfumes);
}

void ParseField(const std::string input, std::string name, int &value)
{
    int curr = input.find(name += ": ");
    value = (curr > 0) ? atoi(&input[curr + name.length()]) : -1;
}

void ParseData(const std::string input, int &number, Data &data)
{
    number = atoi(&input[strlen("Sue ")]);

    ParseField(input, "children", data.children);
    ParseField(input, "cats", data.cats);
    ParseField(input, "samoyeds", data.samoyeds);
    ParseField(input, "pomeranians", data.pomeranians);
    ParseField(input, "akitas", data.akitas);
    ParseField(input, "vizslas", data.vizslas);
    ParseField(input, "goldfish", data.goldfish);
    ParseField(input, "trees", data.trees);
    ParseField(input, "cars", data.cars);
    ParseField(input, "perfumes", data.perfumes);
}

void _tmain(int argc, _TCHAR *argv[])
{
    Data answer{ 3, 7, 2, 3, 0, 0, 5, 3, 2, 1 };

    std::ifstream f("Input.txt");

    for (std::string line; getline(f, line);)
    {
        int number;
        Data data;
        ParseData(line, number, data);

        if (MatchesPart1(data, answer))
        {
            std::cout << "part one: " << number << std::endl;
            assert(number == 103);
        }

        if (MatchesPart2(data, answer))
        {
            std::cout << "part two: " << number << std::endl;
            assert(number == 405);
        }
    }
}
