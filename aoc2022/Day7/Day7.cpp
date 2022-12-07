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

struct Folder
{
    std::string name;
    long long size = 0;
    Folder *parent;
    std::vector<Folder> folders;
    std::vector<File> files;
};

long long FillInFolderSizes(Folder *folder)
{
    for (Folder &f : folder->folders)
        folder->size += FillInFolderSizes(&f);

    for (File &file : folder->files)
        folder->size += file.size;

    return folder->size;
}

void SumFoldersUnderLimit(Folder *folder, long long limit, long long &sum)
{
    if (folder->size <= limit)
        sum += folder->size;

    for (Folder &f : folder->folders)
        SumFoldersUnderLimit(&f, limit, sum);
}

void FindSmallestFolderAtLeastAmount(Folder *folder, long long amount, long long &smallest)
{
    if (folder->size >= amount && folder->size < smallest)
        smallest = folder->size;

    for (Folder &f : folder->folders)
        FindSmallestFolderAtLeastAmount(&f, amount, smallest);
}

Folder root{ "/", 0, nullptr };
Folder *curr = &root;

auto read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string dollar, cd, slash;

    file >> dollar >> cd >> slash >> dollar;

    while (dollar == "$")
    {
        std::string cmd, param;

        file >> cmd;
        if (cmd == "cd")
        {
            file >> param;

            if (param == "..")
                curr = curr->parent;

            for (Folder &f : curr->folders)
                if (f.name == param)
                {
                    curr = &f;
                    break;
                }
        }
        else if (cmd == "ls")
        {
            std::string a, b;
            while (file >> a && a != "$")
            {
                file >> b;

                if (a == "dir")
                    curr->folders.push_back(Folder{ b, 0, curr });
                else
                    curr->files.push_back(File{ b, atoi(a.c_str()) });
            }

            dollar = a;
        }
    }

    FillInFolderSizes(&root);
}

auto do_part1()
{
    long long part1 = 0;
    SumFoldersUnderLimit(&root, 100000, part1);
    return part1;
}

auto do_part2()
{
    auto total_space = 70000000LL, need_unused = 30000000LL;
    auto have_unused = total_space - root.size;
    auto size_to_free_up = need_unused - have_unused;

    auto retval = LLONG_MAX;
    FindSmallestFolderAtLeastAmount(&root, size_to_free_up, retval);
    return retval;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 1086293);
    assert(part2 == 366028);
    return 0;
}
