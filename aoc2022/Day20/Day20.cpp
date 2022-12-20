#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

using namespace std;

struct Info
{
    long long value;
    int original_pos;
};

std::vector<Info> input;

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    int n, i = 0;
    int pos = 0;

    while (file >> n)
        input.push_back(Info{ n, pos++ });

    for (int i = 0; i < input.size(); i++)
    {
        auto start = -1;
        for (int j = 0; j < input.size(); j++)
            if (input[j].original_pos == i)
            {
                start = j;
                break;
            }

        auto moves = input[start].value % ((int)input.size() - 1);
        auto end = start + moves;
        if (end <= 0)
            end += (input.size() - 1);
        else if (end >= input.size())
            end -= (input.size() - 1);

        if (end > start) // right
        {
            while (start != end)
            {
                auto temp = input[start];
                input[start] = input[start + 1];
                input[start + 1] = temp;
                start++;
            }
        }
        else // left
        {
            while (start != end)
            {
                auto temp = input[start];
                input[start] = input[start - 1];
                input[start - 1] = temp;
                start--;
            }
        }
    }

    auto zero_pos = -1;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i].value == 0)
        {
            zero_pos = i;
            break;
        }
    }

    auto first = input[(zero_pos + 1000) % input.size()].value;
    auto second = input[(zero_pos + 2000) % input.size()].value;
    auto third = input[(zero_pos + 3000) % input.size()].value;

    return first + second + third;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    long long n, i = 0;
    int pos = 0;

    while (file >> n)
        input.push_back(Info{ n * 811589153, pos++ });

    for (int mixes = 1; mixes <= 10; mixes++)
    {
        for (int i = 0; i < input.size(); i++)
        {
            //cout << "Step @index " << i << endl;

            auto start = -1;
            for (int j = 0; j < input.size(); j++)
                if (input[j].original_pos == i)
                {
                    start = j;
                    break;
                }

            auto moves = input[start].value % ((long long)input.size() - 1);
            if (moves != 0)
            {
                auto end = start + moves;
                if (end <= 0)
                    end += ((long long)input.size() - 1);
                else if (end >= (long long)input.size())
                    end -= ((long long)input.size() - 1);

                if (end > start) // right
                {
                    while (start != end)
                    {
                        auto temp = input[start];
                        input[start] = input[start + 1];
                        input[start + 1] = temp;
                        start++;
                    }
                }
                else if (end < start) // left
                {
                    while (start != end)
                    {
                        auto temp = input[start];
                        input[start] = input[start - 1];
                        input[start - 1] = temp;
                        start--;
                    }
                }
            }
        }
    }

    auto zero_pos = -1;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i].value == 0)
        {
            zero_pos = i;
            break;
        }
    }

    auto first = input[(zero_pos + 1000) % input.size()].value;
    auto second = input[(zero_pos + 2000) % input.size()].value;
    auto third = input[(zero_pos + 3000) % input.size()].value;

    return first + second + third;
}

int main()
{
    //auto part1 = do_part1("input.txt");
    //std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    //assert(part1 == 8372);
    assert(part2 == 7865110481723);
    return 0;
}
