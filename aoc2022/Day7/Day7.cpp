#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>

struct File
{
    std::string name;
    long long size;
};

struct Directory
{
    std::string name;
    long long size;
    Directory *parent;
    std::vector<Directory> directories;
    std::vector<File> files;
};

Directory root{ "", 0, nullptr, { Directory{"/", 0, &root} } };

long long FillInDirectorySizes(Directory &directory = root)
{
    for (auto &d : directory.directories)
        directory.size += FillInDirectorySizes(d);

    for (auto &f : directory.files)
        directory.size += f.size;

    return directory.size;
}

void SumDirectoriesUnderLimit(Directory &directory, long long limit, long long &sum)
{
    for (auto &d : directory.directories)
        SumDirectoriesUnderLimit(d, limit, sum);

    if (directory.size <= limit)
        sum += directory.size;
}

void FindSmallestDirectorySizeAtLeastAmount(Directory &directory, long long amount, long long &smallest)
{
    for (auto &d : directory.directories)
        FindSmallestDirectorySizeAtLeastAmount(d, amount, smallest);

    if (directory.size >= amount && directory.size < smallest)
        smallest = directory.size;
}

auto process_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string s, cmd, param, name;
    auto *curr = &root;

    file >> s;

    while (s == "$")
    {
        file >> cmd;

        if (cmd == "cd")
        {
            file >> param;

            if (param == "..")
                curr = curr->parent;
            else
                for (Directory &d : curr->directories)
                    if (d.name == param)
                    {
                        curr = &d;
                        break;
                    }
        }
        else if (cmd == "ls")
        {
            while (file >> s && s != "$")
            {
                file >> name;

                if (s == "dir")
                    curr->directories.push_back(Directory{ name, 0, curr });
                else
                    curr->files.push_back(File{ name, atoi(s.c_str()) });
            }
        }
    }

    FillInDirectorySizes();
}

auto do_part1()
{
    auto retval = 0LL;
    SumDirectoriesUnderLimit(root, 100000, retval);
    return retval;
}

auto do_part2()
{
    auto total_space = 70000000LL, need_unused = 30000000LL;
    auto have_unused = total_space - root.size;
    auto size_to_free_up = need_unused - have_unused;

    auto retval = LLONG_MAX;
    FindSmallestDirectorySizeAtLeastAmount(root, size_to_free_up, retval);
    return retval;
}

int main()
{
    process_input("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 1086293);
    assert(part2 == 366028);
    return 0;
}
