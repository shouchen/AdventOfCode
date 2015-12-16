// AuntSue.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

struct Data
{
    int children, cats, samoyeds, pomeranians, akitas;
    int vizslas, goldfish, trees, cars, perfumes;

    bool Matches1(const Data &data)
    {
        return
            FieldMatches(children, data.children) &&
            FieldMatches(cats, data.cats) &&
            FieldMatches(samoyeds, data.samoyeds) &&
            FieldMatches(pomeranians, data.pomeranians) &&
            FieldMatches(akitas, data.akitas) &&
            FieldMatches(vizslas, data.vizslas) &&
            FieldMatches(goldfish, data.goldfish) &&
            FieldMatches(trees, data.trees) &&
            FieldMatches(cars, data.cars) &&
            FieldMatches(perfumes, data.perfumes);
    }

    bool Matches2(const Data &data)
    {
        return
            FieldMatches(children, data.children) &&
            (cats == -1 || data.cats == -1 || cats > data.cats) &&
            FieldMatches(samoyeds, data.samoyeds) &&
            (pomeranians == -1 || data.pomeranians == -1 || pomeranians < data.pomeranians) &&
            FieldMatches(akitas, data.akitas) &&
            FieldMatches(vizslas, data.vizslas) &&
            (goldfish == -1 || data.goldfish == -1 || goldfish < data.goldfish) &&
            (trees == -1 || data.trees == -1 || trees > data.trees) &&
            FieldMatches(cars, data.cars) &&
            FieldMatches(perfumes, data.perfumes);
    }

    bool FieldMatches(int left, int right)
    {
        return left == -1 || right == -1 || left == right;
    }
};

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

Data answer{ 3, 7, 2, 3, 0, 0, 5, 3, 2, 1 };  // ans: 103

void _tmain(int argc, _TCHAR *argv[])
{
    std::ifstream f("Input.txt");

    for (std::string line; getline(f, line);)
    {
        int number;
        Data data;
        ParseData(line, number, data);

        if (data.Matches1(answer))
            std::cout << "part one: " << number << std::endl;

        if (data.Matches2(answer))
            std::cout << "part two: " << number << std::endl;
    }

    f.close();
}
